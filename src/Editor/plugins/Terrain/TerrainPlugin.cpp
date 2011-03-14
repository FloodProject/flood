/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"
#include "TerrainPlugin.h"
#include "TerrainPage.h"
#include "TerrainOperation.h"
#include "Utilities.h"
#include "Editor.h"
#include "EditorIcons.h"
#include "Viewframe.h"
#include "UndoManager.h"
#include "Render/View.h"
#include "Scene/Camera.h"
#include "Input/InputManager.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "Engine.h"

#ifdef ALL_PLUGINS

namespace vapor { namespace editor {

//-----------------------------------//

REFLECT_CHILD_CLASS(TerrainPlugin, Plugin)
REFLECT_END()

//-----------------------------------//

BEGIN_EVENT_TABLE(TerrainPlugin, wxEvtHandler)
	EVT_TIMER(wxID_ANY, TerrainPlugin::onTimer)
END_EVENT_TABLE()

//-----------------------------------//

TerrainPlugin::TerrainPlugin()
	: Plugin()
	, terrainPage(nullptr)
	, terrainOperation(nullptr)
	, timer(this)
{ }

//-----------------------------------//

PluginMetadata TerrainPlugin::getMetadata()
{
	PluginMetadata metadata;
	
	metadata.name = "Terrains";
	metadata.description = "Provides terrain editing tools.";
	metadata.author = "triton";
	metadata.version = "1.0";

	return metadata;
}

//-----------------------------------//

void TerrainPlugin::onPluginEnable()
{
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

	terrainPage = new TerrainPage(editor);

	wxBitmap icon = wxMEMORY_BITMAP(world);
	
	wxAuiPaneInfo pane;	
	pane.Caption("Terrains").Left().Dock().Hide().Icon(icon);
	pane.BestSize(200, -1);

	editor->getAUI()->AddPane(terrainPage, pane);
	editor->getAUI()->Update();
}

//-----------------------------------//

void TerrainPlugin::onToolSelect( int id )
{
	delete terrainOperation;
	terrainOperation = nullptr;

	tool = (TerrainTool::Enum) id;

	wxAuiPaneInfo& pane = editor->getAUI()->GetPane("Terrains");
	
	if( !pane.IsShown() )
	{
		pane.Show();
		editor->getAUI()->Update();
	}
}

//-----------------------------------//

void TerrainPlugin::onToolUnselect( int id )
{
	if(isPluginTool(id))
		return;

	editor->getAUI()->GetPane("Terrains").Hide();
	editor->getAUI()->Update();
}

//-----------------------------------//

void TerrainPlugin::onPluginDisable()
{
	//removePage( terrainPage );
}

//-----------------------------------//

void TerrainPlugin::onEntitySelect( const EntityPtr& node )
{
	if( !node->getType().is<Terrain>() )
		return;

	terrain = std::static_pointer_cast<Terrain>(node);
}

//-----------------------------------//

void TerrainPlugin::onEntityUnselect( const EntityPtr& )
{

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
		editor->redrawView();
	}
}

//-----------------------------------//

void TerrainPlugin::onMouseButtonPress( const MouseButtonEvent& mbe )
{
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
	registerUndoOperation();
}

//-----------------------------------//

void TerrainPlugin::onMouseDrag( const MouseDragEvent& mde )
{
	//if( !mde.info.leftButton )
	//	return;

	MouseButtonEvent mbe( MouseEventType::MousePress );
	mbe.x = mde.x;
	mbe.y = mde.y;
	mbe.button = MouseButton::Left;

	// We need to handle the case where the user clicks but doesn't move
	// the mouse anymore. As we don't receive further events in this case,
	// we check if the mouse is still pressed.

	setupOperation(mbe);
}

//-----------------------------------//

void TerrainPlugin::onMouseLeave()
{
	// We use this event to check if the user leaves the window while
	// dragging in the middle of a terrain operation. If that isn't
	// the case, then we've got nothing to do here.
	if( !terrainOperation )
		return;

	registerUndoOperation();
}

//-----------------------------------//

bool TerrainPlugin::pickCell( const MouseButtonEvent& mbe )
{
	if( !terrain )
		return false;

	RenderView* view = viewframe->getView();
	const CameraPtr& camera = view->getCamera(); 
	
	// Get a ray given the screen location clicked.
	const Ray& pickRay = camera->getRay( mbe.x, mbe.y );

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

	editor->redrawView();
}

//-----------------------------------//

void TerrainPlugin::onRebuildNormals( wxCommandEvent& event )
{
	CellPtr cell = terrain->getCell(coords.x, coords.y);
	assert( cell != nullptr );

	cell->rebuildNormals();

	editor->redrawView();
}

//-----------------------------------//

void TerrainPlugin::createOperation( const RayTriangleQueryResult& res )
{
	if( terrainOperation )
		return;

	InputManager* input = engine->getInputManager();

	// If the left Shift is held down, then lower.
	Keyboard* keyboard = input->getKeyboard();
	bool raise = !keyboard->isKeyPressed( Keys::LShift );	

	terrainOperation = new TerrainOperation( tool, res );
	terrainOperation->terrain = terrain;

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
	if( !terrainOperation )
		return;

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

	if( !pickTerrain(mb, res) )
		return;

	EntityPtr parent = res.entity->getParent()->getShared();
	terrain = std::static_pointer_cast<Terrain>(parent);

	if( !terrainOperation )
		createOperation(res);
	else
		terrainOperation->rayQuery = res;

	terrainOperation->applyTool();
	editor->redrawView();

	timer.Start( TERRAIN_TIMER_MS );
}

//-----------------------------------//

bool TerrainPlugin::pickTerrain( const MouseButtonEvent& mb, RayTriangleQueryResult& res )
{
	const ScenePtr& scene = engine->getSceneManager();

	RenderView* view = viewframe->getView();
	const CameraPtr& camera = view->getCamera(); 
	
	// Get a ray given the screen location clicked.
	const Ray& pickRay = camera->getRay( mb.x, mb.y );

	RayQueryResult query;
	
	if( !scene->doRayBoxQuery(pickRay, query) )
		return false;

	const EntityPtr& entity = query.entity;
	
	if( !entity ) 
		return false;

	if( !entity->getParent()->getType().is<Terrain>() )
		return false;

	if( !scene->doRayTriangleQuery(pickRay, res, entity) )
		return false; // Query does not intersect the terrain.

	return true;
}

//-----------------------------------//

void TerrainPlugin::createContextMenu( const MouseButtonEvent& mbe )
{
	std::string menuTitle( "Terrain Operations" );
	wxMenu menu( menuTitle );

	if( pickCell(mbe) )
	{
		CellPtr cell = terrain->getCell(coords.x, coords.y);

		std::string newTitle = String::format( "%s (Cell %hd,%hd)",
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

} } // end namespaces

#endif