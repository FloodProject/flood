/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"

#ifdef ENABLE_PLUGIN_TERRAIN

#include "TerrainPlugin.h"
#include "TerrainPage.h"
#include "TerrainOperation.h"
#include "Core/Utilities.h"
#include "Core/Profiler.h"
#include "Editor.h"
#include "EditorIcons.h"
#include "DocumentWindow.h"
#include "UndoManager.h"
#include "Render/View.h"
#include "Scene/Camera.h"
#include "Input/InputManager.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "Scene/Projector.h"
#include "Engine/Engine.h"
#include "EventManager.h"
#include "RenderControl.h"

#include "../Scene/SceneDocument.h"

#define TERRAIN_PANE_NAME "Terrains"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(TerrainPlugin, EditorPlugin)
REFLECT_CLASS_END()

//-----------------------------------//

TerrainPlugin::TerrainPlugin()
	: terrainPage(nullptr)
	, terrainOperation(nullptr)
	, terrain(nullptr)
	, entityProjector(nullptr)
{
	timer.Bind(wxEVT_TIMER, &TerrainPlugin::onTimer, this);
}

//-----------------------------------//

PluginMetadata TerrainPlugin::getMetadata()
{
	PluginMetadata metadata;
	
	metadata.name = "Terrains";
	metadata.description = "Provides terrain editing tools.";
	metadata.author = "triton";
	metadata.version = "1.0";
	metadata.priority = 330;

	return metadata;
}

//-----------------------------------//

void TerrainPlugin::onPluginEnable()
{
#ifdef ENABLE_TERRAIN_TOOLS_ON_TOOLBAR
	wxAuiToolBar* toolbarCtrl = editor->getToolbar();
	
	if(toolbarCtrl)
	{
		buttonRaise = toolbarCtrl->AddTool( TerrainTool::Raise, "Terrain Raise",
			wxMEMORY_BITMAP(terrain_raise_lower), "Raises/Lowers the terrain",
			wxITEM_RADIO );
		addTool(buttonRaise, true);

		buttonPaint = toolbarCtrl->AddTool( TerrainTool::Paint, "Terrain Paint",
			wxMEMORY_BITMAP(terrain_paint), "Paints the terrain", wxITEM_RADIO );
		addTool(buttonPaint, true);
	}
#endif

	terrainPage = new TerrainPage(editor);

	wxBitmap icon = wxMEMORY_BITMAP(world);
	
	wxAuiPaneInfo pane;
	pane.Name(TERRAIN_PANE_NAME).Caption(TERRAIN_PANE_NAME).Hide().Left().Icon(icon);
	pane.BestSize(200, -1);

	editor->getAUI()->AddPane(terrainPage, pane);
	editor->getAUI()->Update();

	editor->getEventManager()->addEventListener(this);
}

//-----------------------------------//

void TerrainPlugin::onToolSelect( int id )
{
	Deallocate(terrainOperation);
	tool = (TerrainTool::Enum) id;

#if PROGRAM_BUFFER
	ProgramManager* programs = GetRenderDevice()->getProgramManager();
	programs->getProgram("ProjectiveTexturing", true);
#endif
}

//-----------------------------------//

void TerrainPlugin::onToolUnselect( int id )
{
	if( hasTool(id) ) return;
}

//-----------------------------------//

void TerrainPlugin::onPluginDisable()
{
	//removePage( terrainPage );
}

//-----------------------------------//

void TerrainPlugin::onEntitySelect( const EntityPtr& entity )
{
	const CellPtr& cell = entity->getComponentFromFamily<Cell>();
	if( !cell ) return;
	
	terrain = (Terrain*) cell->getEntity()->getParent();

	if( !ReflectionIsEqual(terrain->getType(), TerrainGetType()) )
		return;

	wxAuiPaneInfo& pane = editor->getAUI()->GetPane(TERRAIN_PANE_NAME);
	
	if( !pane.IsShown() )
	{
		pane.Show();
		editor->getAUI()->Update();
	}
}

//-----------------------------------//

void TerrainPlugin::onEntityUnselect( const EntityPtr& entity )
{
	const CellPtr& cell = entity->getComponentFromFamily<Cell>();
	if( !cell ) return;
	
	Terrain* newTerrain = (Terrain*) cell->getEntity()->getParent();

	if( !ReflectionIsEqual(newTerrain->getType(), TerrainGetType()) )
		return;

	if( newTerrain != terrain )
		return;

	terrain = nullptr;

	editor->getAUI()->GetPane("Terrains").Hide();
	editor->getAUI()->Update();
}

//-----------------------------------//

void TerrainPlugin::onSceneUnload( const ScenePtr& scene )
{
	terrain = nullptr;
	terrainOperation = nullptr;
	entityProjector = nullptr;
}

//-----------------------------------//

void TerrainPlugin::onTimer( wxTimerEvent& )
{
	Mouse* mouse = GetEngine()->getInputManager()->getMouse();
	const MouseInfo& info = mouse->getMouseInfo();

	if( info.leftButton )
	{
		assert( terrainOperation != nullptr );

		terrainOperation->applyTool();
		editor->getDocument()->getWindow()->flagRedraw();
	}
}

//-----------------------------------//

void TerrainPlugin::onMouseButtonPress( const MouseButtonEvent& mbe )
{
	if( !terrain ) return;

	if( mbe.isLeftButton() )
	{
		setupOperation(mbe);
	}
	else if( mbe.isRightButton() )
	{
		createContextMenu(mbe);
	}
}

//-----------------------------------//

void TerrainPlugin::onMouseButtonRelease( const MouseButtonEvent& )
{
	if( !terrain ) return;
	registerUndoOperation();
}

//-----------------------------------//

void TerrainPlugin::onMouseDrag( const MouseDragEvent& mde )
{
	if( !terrain ) return;

	MouseButtonEvent mbe( MouseEventType::MousePress );
	mbe.x = mde.x;
	mbe.y = mde.y;
	mbe.button = MouseButton::Left;

	// We need to handle the case where the user clicks but doesn't move
	// the mouse anymore. As we don't receive further events in this case,
	// we check if the mouse is still pressed.
	
	if( mde.info->leftButton ) setupOperation(mbe);

	updateBrushProjection(mbe.x, mbe.y);

}

//-----------------------------------//

void TerrainPlugin::onMouseLeave()
{
	if( !terrain ) return;

	// We use this event to check if the user leaves the window while
	// dragging in the middle of a terrain operation. If that isn't
	// the case, then we've got nothing to do here.
	
	if( !terrainOperation ) return;

	registerUndoOperation();
}

//-----------------------------------//

void TerrainPlugin::onMouseMove( const MouseMoveEvent& mb )
{
	if( !terrain ) return;

	updateBrushProjection(mb.x, mb.y);
}

//-----------------------------------//

void TerrainPlugin::updateBrushProjection( int x, int y )
{
	Viewframe* viewframe = (Viewframe*) GetEditor().getDocument()->getWindow();
	RenderWindow* window = viewframe->getControl()->getRenderWindow();

	RayTriangleQueryResult res;
	bool picked = pickTerrain(x, y, res);
	
	if( !picked && !entityProjector )
		return;

	if( !picked )
	{
		SceneDocument* document = (SceneDocument*) GetEditor().getDocument();
		document->editorScene->remove(entityProjector);
		entityProjector = nullptr;
	}
	else
	{
		const TerrainCellPtr& cell = RefCast<TerrainCell>(res.geometry);
		projectBrush(res.intersectionWorld, cell);
	}
}

//-----------------------------------//

void TerrainPlugin::projectBrush(const Vector3& pos, const TerrainCellPtr& cell)
{
	ImageHandle brushHandle = terrainPage->getBrushImage();
	
	Image* brushImage = brushHandle.Resolve();
	if( !brushImage ) return;

	if( !entityProjector )
	{
		EntityPtr projector = createProjector(cell);
		entityProjector = projector.get();
		
		SceneDocument* document = (SceneDocument*) GetEditor().getDocument();
		document->editorScene->add(projector);
	}

	const ProjectorPtr& projector = entityProjector->getComponent<Projector>();

	float relativeSize = terrainPage->getBrushSize() / float(BRUSH_INITIAL_SIZE);
	Vector3 brushImageSize( brushImage->getWidth(), brushImage->getHeight(), 0 );
	projector->frustum.orthoSize = brushImageSize * relativeSize;

	// Update the position of the projector.
	const TransformPtr& transform = entityProjector->getTransform();
	Vector3 frustumSize = projector->frustum.orthoSize / 2;
	Vector3 newPos = pos - Vector3(frustumSize.x, 0, frustumSize.y);
	transform->setPosition(newPos);

	//LogDebug("New projector position: %f, %f, %f", newPos.x, newPos.y, newPos.z);

	Material* material = projector->material.Resolve();
	material->setTexture(0, brushHandle);
	material->getTextureUnit(0).wrap = TextureWrapMode::ClampToBorder;
}

//-----------------------------------//

EntityPtr TerrainPlugin::createProjector(const GeometryPtr& cell)
{
	MaterialHandle materialHandle = MaterialCreate(AllocatorGetHeap(), "ProjectMaterial");
		
	Material* material = materialHandle.Resolve();
	material->setProgram("ProjectiveTexturing");
	material->setBlending(BlendSource::SourceAlpha, BlendDestination::InverseSourceAlpha);
	material->setDepthCompare(DepthCompare::LessOrEqual);

	EntityPtr entity = EntityCreate( AllocatorGetThis() );
	entity->setName("Projector");
	entity->addTransform();
	
	ProjectorPtr projector = AllocateThis(Projector);
	projector->material = materialHandle;
	projector->geometry = cell;
	projector->frustum.projection = Projection::Orthographic;	
	entity->addComponent(projector);

	Quaternion quat;
	quat.setToRotateAboutX(MathDegreeToRadian(90.0f));
		
	const TransformPtr& transform = entity->getTransform();		
	transform->setRotation(quat);

	return entity;
}

//-----------------------------------//

bool TerrainPlugin::pickCell( int x, int y )
{
	if( !terrain ) return false;

	SceneDocument* document = (SceneDocument*) GetEditor().getDocument();
	const CameraPtr& camera = document->viewframe->getView()->getCamera(); 
	
	// Get a ray given the screen location clicked.
	const Ray& pickRay = camera->getRay(x, y);

	Plane ground( Vector3::UnitY, 0 );
	
	float distance;
	if( !ground.intersects(pickRay, distance) )
		return false;
		
	Vector3 pt = pickRay.getPoint(distance);
	coords = terrain->getCoords(pt);
		
	return true;
}

//-----------------------------------//

void TerrainPlugin::onCreateCell( wxCommandEvent& event )
{
	terrain->addCell(coords.x, coords.y);

	editor->getDocument()->getWindow()->flagRedraw();
}

//-----------------------------------//

void TerrainPlugin::onRebuildNormals( wxCommandEvent& event )
{
	CellPtr cell = terrain->getCell(coords.x, coords.y);
	assert( cell != nullptr );

	cell->rebuildNormals();

	editor->getDocument()->getWindow()->flagRedraw();
}

//-----------------------------------//

void TerrainPlugin::createOperation( const RayTriangleQueryResult& res )
{
	if( terrainOperation ) return;

	InputManager* input = GetInputManager();

	// If the left Shift is held down, then lower.
	Keyboard* keyboard = input->getKeyboard();
	bool raise = !keyboard->isKeyPressed( Keys::LShift );	

	terrainOperation = AllocateThis(TerrainOperation, tool, res);
	terrainOperation->description = "Terrain editing";
	terrainOperation->terrain = terrain;

	terrainOperation->brush = terrainPage->getBrushImage().Resolve();
	terrainOperation->brushSize = terrainPage->getBrushSize();
	terrainOperation->brushStrength = terrainPage->getBrushStrength();
	terrainOperation->paintImage = terrainPage->getPaintImage();

	terrainOperation->tileLock = terrainPage->getTileLock();
	terrainOperation->tileOffsetX = terrainPage->getTileOffsetX();
	terrainOperation->tileOffsetY = terrainPage->getTileOffsetY();
}

//-----------------------------------//

void TerrainPlugin::registerUndoOperation()
{
	if( !terrainOperation ) return;

	timer.Stop();

	// Register the operation in the stack so it can be undone later.
	terrainOperation->ready();
	
	UndoManager* undo = editor->getDocument()->getUndoManager();
	undo->registerOperation( terrainOperation );

	terrainOperation = nullptr;
}

//-----------------------------------//

void TerrainPlugin::setupOperation( const MouseButtonEvent& mb )
{
	const int TERRAIN_TIMER_MS = 500;

	RayTriangleQueryResult res;

	if( !pickTerrain(mb.x, mb.y, res) )
		return;

	EntityPtr parent = res.entity->getParent();
	terrain = RefCast<Terrain>(parent).get();

	if( !terrainOperation )
		createOperation(res);
	else
		terrainOperation->rayQuery = res;

	terrainOperation->applyTool();
	editor->getDocument()->getWindow()->flagRedraw();

	timer.Start( TERRAIN_TIMER_MS );
}

//-----------------------------------//

bool TerrainPlugin::pickTerrain( int x, int y, RayTriangleQueryResult& res )
{
	SceneDocument* document = (SceneDocument*) GetEditor().getDocument();
	const ScenePtr& scene = document->scene;
	const CameraPtr& camera = document->viewframe->getView()->getCamera(); 
	
	// Get a ray given the screen location clicked.
	const Ray& pickRay = camera->getRay(x, y);

	RayQueryResult query;
	
	if( !scene->doRayBoxQuery(pickRay, query) )
		return false;

	const EntityPtr& entity = query.entity;
	
	if( !entity ) 
		return false;

	if( !entity->getComponentFromFamily<Cell>() )
		return false;

	if( !scene->doRayTriangleQuery(pickRay, res, entity) )
		return false; // Query does not intersect the terrain.

	return true;
}

//-----------------------------------//

void TerrainPlugin::createContextMenu( const MouseButtonEvent& mbe )
{
	String menuTitle( "Terrain UndoOperations" );
	wxMenu menu( menuTitle );

	if( pickCell(mbe.x, mbe.y) )
	{
		CellPtr cell = terrain->getCell(coords.x, coords.y);

		String newTitle = StringFormat( "%s (Cell %hd,%hd)",
			menuTitle.c_str(), coords.x, coords.y );

		menu.SetTitle( newTitle );

		if( cell )
		{
			wxMenuItem* rebuildNormalsButton =
				menu.Append(wxID_ANY, "Rebuild normals");

			menu.Bind( wxEVT_COMMAND_MENU_SELECTED,
				&TerrainPlugin::onRebuildNormals, this,
				rebuildNormalsButton->GetId() );

			// TODO:
			//wxMenuItem* removeCellButton =
			//	menu.Append(wxID_ANY, "Remove cell");

			//menu.Bind( wxEVT_COMMAND_MENU_SELECTED,
			//	&TerrainPlugin::onRemoveCell, this,
			//	removeCellButton->GetId() );
		}
		else
		{
			wxMenuItem* createCellButton =
				menu.Append(wxID_ANY, "Create cell");

			menu.Bind( wxEVT_COMMAND_MENU_SELECTED,
				&TerrainPlugin::onCreateCell, this,
				createCellButton->GetId() );
		}
	}
	else
	{
		if( !terrain )
			menu.Append(wxID_ANY, "Invalid terrain" );
		else
			menu.Append(wxID_ANY, "Invalid cell" );
	}

	wxPoint clientpt( mbe.x, mbe.y );
	editor->PopupMenu(&menu, clientpt);
}

//-----------------------------------//

NAMESPACE_EDITOR_END

#endif