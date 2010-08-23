/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "TerrainPlugin.h"
#include "TerrainPage.h"
#include "Editor.h"
#include "UndoManager.h"
#include "EditorIcons.h"
#include "Viewframe.h"

namespace vapor { namespace editor {

//-----------------------------------//

BEGIN_EVENT_TABLE(TerrainPlugin, wxEvtHandler)
	EVT_TIMER(wxID_ANY, TerrainPlugin::onTimer)
END_EVENT_TABLE()

//-----------------------------------//

TerrainPlugin::TerrainPlugin( EditorFrame* frame )
	: Plugin(frame)
	, terrainPage(nullptr)
	, timer(this)
	, op(nullptr)
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
	wxToolBar* toolBar = editor->getToolbar();
	wxToolBarToolBase* base = nullptr;
	
	base = toolBar->AddTool( wxID_ANY, "Raise/Lower",
		wxMEMORY_BITMAP(terrain_raise_lower), "Raises/Lowers the terrain",
		wxITEM_RADIO );
	//base->Enable(false);
	addTool(base);

	base = toolBar->AddTool( wxID_ANY, "Paint", wxMEMORY_BITMAP(terrain_paint),
		"Paints the terrain", wxITEM_RADIO );
	//base->Enable(false);
	addTool(base);

	wxNotebook* notebookCtrl = editor->getNotebook();
	wxImageList* imageList = notebookCtrl->GetImageList();

	wxBitmap iconWorld = wxMEMORY_BITMAP(world);
	int iconTerrain = imageList->Add(iconWorld);

	terrainPage = new TerrainPage( engine, notebookCtrl );
	notebookCtrl->AddPage( terrainPage, wxEmptyString/*wxT("Terrains")*/, false, iconTerrain );
}

//-----------------------------------//

void TerrainPlugin::onPluginDisable()
{
	removePage( terrainPage );
}

//-----------------------------------//

void TerrainPlugin::onNodeSelect( const NodePtr& node )
{
	foreach( const ComponentMapPair p, node->getComponents() )
	{
		if( p.first->is<Terrain>() )
		{
			terrain = std::static_pointer_cast<Terrain>(p.second);

			//foreach( wxToolBarToolBase* tool, tools )
			//	tool->Enable(true);
		}
	}
}

//-----------------------------------//

void TerrainPlugin::onNodeUnselect( const NodePtr& )
{

}

//-----------------------------------//

void TerrainPlugin::onTimer( wxTimerEvent& )
{
	Mouse* mouse = engine->getInputManager()->getMouse();
	const MouseInfo& info = mouse->getMouseInfo();

	if( info.leftButton )
	{
		assert( op != nullptr );

		op->applyTerrainTool();
		editor->RefreshViewport();
	}
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

	deformTerrain( mbe );
}

//-----------------------------------//

bool TerrainPlugin::pickCell( const MouseButtonEvent& mbe )
{
	if( !terrain )
		return false;

	View* view = viewframe->getView();
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

void TerrainPlugin::onMouseButtonPress( const MouseButtonEvent& mbe )
{
	if( mbe.isLeftButton() )
	{
		deformTerrain( mbe );
	}
	else if( mbe.isRightButton() )
	{
		createContextMenu( mbe );
	}
}

//-----------------------------------//

void TerrainPlugin::onCreateCell( wxCommandEvent& event )
{
	terrain->addCell(coords.x, coords.y);

	editor->RefreshViewport();
}

//-----------------------------------//

void TerrainPlugin::onRebuildNormals( wxCommandEvent& event )
{
	CellPtr cell = terrain->getCell(coords.x, coords.y);
	assert( cell != nullptr );

	cell->rebuildNormals();

	editor->RefreshViewport();
}


//-----------------------------------//

void TerrainPlugin::onMouseButtonRelease( const MouseButtonEvent& )
{
	registerEvent();
}

//-----------------------------------//

void TerrainPlugin::onMouseLeave()
{
	// We use this event to check if the user leaves the window while
	// dragging in the middle of a terrain operation. If that isn't
	// the case, then we've got nothing to do here.
	if( !op ) return;

	registerEvent();
}

//-----------------------------------//

void TerrainPlugin::createOperation( const RayTriangleQueryResult& res )
{
	if( op )
		return;

	// If the left Shift is held down, then lower.
	Keyboard* kbd = engine->getInputManager()->getKeyboard();
	bool raise = !kbd->isKeyPressed( Keys::LShift );	

	TerrainTool::Enum tool;
	tool = raise ? TerrainTool::Raise : TerrainTool::Lower;

	op = new TerrainOperation( tool, res );
	op->size = 50;
	op->strength = 2;
}

//-----------------------------------//

void TerrainPlugin::registerEvent()
{
	if( !op )
		return;

	timer.Stop();

	// Register the operation in the stack so it can be undone later.
	op->ready();
	
	UndoManager* undoManager = editor->getUndoManager();
	undoManager->registerOperation( op );

	op = nullptr;
}

//-----------------------------------//

void TerrainPlugin::deformTerrain( const MouseButtonEvent& mb )
{
	static const int TERRAIN_TIMER_MS = 100;

	RayTriangleQueryResult res;
	
	if( !pickTerrain(mb, res) )
		return;

	if( !op )
		createOperation(res);
	else
		op->res = res;

	op->applyTerrainTool();
	editor->RefreshViewport();

	timer.Start( TERRAIN_TIMER_MS );
}

//-----------------------------------//

bool TerrainPlugin::pickTerrain( const MouseButtonEvent& mb,
							     RayTriangleQueryResult& res )
{
	const ScenePtr& scene = engine->getSceneManager();

	View* view = viewframe->getView();
	const CameraPtr& camera = view->getCamera(); 
	
	// Get a ray given the screen location clicked.
	const Ray& pickRay = camera->getRay( mb.x, mb.y );

	RayBoxQueryResult query;
	
	if( !scene->doRayBoxQuery( pickRay, query ) )
		return false;

	const NodePtr& node = query.node;
	
	if( !node ) 
		return false;

	const TerrainPtr& terrain = node->getComponent<Terrain>("Terrain");
	
	if( !terrain )
		return false; // Picked nodes were not terrain.

	if( !scene->doRayTriangleQuery(pickRay, res, node) )
		return false; // Query does not intersect the terrain.

	return true;
}

//-----------------------------------//

TerrainOperation::TerrainOperation( TerrainTool::Enum tool,
								    const RayTriangleQueryResult& res )
	: size(0)
	, strength(0)
	, tool(tool)
	, res(res)
{
	processState( beforeHeights, true );
}

//-----------------------------------//

void TerrainOperation::ready()
{
	processState( afterHeights, true );
	updateNormals();
}

//-----------------------------------//

void TerrainOperation::undo()
{
	processState( beforeHeights, false );
	updateNormals();
}

//-----------------------------------//

void TerrainOperation::redo()
{
	// If the operation can't be undone with an algorithm, 
	// then our best option is to save the state of the terrain.

	processState( afterHeights, false );
	updateNormals();
}

//-----------------------------------//

void TerrainOperation::updateNormals()
{
	RenderablePtr renderable = res.renderable;
	assert( renderable != nullptr );

	#pragma TODO("Generate terrain normals in the background")
	CellPtr cell = boost::static_pointer_cast<Cell>( renderable );
	cell->rebuildNormals();
}

//-----------------------------------//

void TerrainOperation::processState( std::vector<float>& heights, bool save )
{	
	RenderablePtr rend = res.renderable;
	
	if( !rend )
		return;

	VertexBufferPtr vb = rend->getVertexBuffer();
	
	if( !vb )
		return;

	if( save )
	{
		// We only need the height of the vertex. This way we can save some memory.
		#pragma TODO("There is no need to save the unmodified heights of the terrain")

		foreach( const Vector3& v, vb->getVertices() )
			heights.push_back( v.y );
	}
	else
	{
		std::vector<Vector3>& vertices = vb->getVertices();

		for( uint i = 0; i < vertices.size(); i++ )
			vertices[i].y = heights[i];

		vb->forceRebuild();
	}
}

//-----------------------------------//

void TerrainOperation::applyTerrainTool()
{
	RenderablePtr rend = res.renderable;
	
	if( !rend )
		return;

	VertexBufferPtr vb = rend->getVertexBuffer();
	
	if( !vb )
		return;

	const Vector3& pick = res.intersection;

	BoundingSphere bs( pick, size );

	bool updateVB = false;

	foreach( Vector3& v, vb->getVertices() )
	{
		if( !bs.intersects(v) )
			continue;

		if( tool == TerrainTool::Raise )
			v.y += strength;
		
		if( tool == TerrainTool::Lower )
			v.y -= strength;

		updateVB = true;
	}

	if( updateVB ) 
		vb->forceRebuild();
}

//-----------------------------------//

} } // end namespaces