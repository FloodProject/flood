/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "TerrainMode.h"
#include "EditorIcons.h"
#include "Editor.h"

namespace vapor { namespace editor {

//-----------------------------------//

enum
{
	Terrain_Raise = 17226,
	Terrain_Paint,
};

const int TERRAIN_TIMER_MS = 100;

//-----------------------------------//

BEGIN_EVENT_TABLE(TerrainMode, wxEvtHandler)
	EVT_TIMER(wxID_ANY, TerrainMode::onTimer)
END_EVENT_TABLE()

//-----------------------------------//

TerrainMode::TerrainMode( EditorFrame* frame )
	: Mode( frame ), timer( this ), op( nullptr )
{
}

//-----------------------------------//

void TerrainMode::onModeInit(wxToolBar* toolBar, ModeIdMap& map)
{
	toolBar->AddTool( Terrain_Raise, "Raise/Lower",
		wxMEMORY_BITMAP(terrain_raise_lower), "Raises/Lowers the terrain",
		wxITEM_RADIO );

	toolBar->AddTool( Terrain_Paint, "Paint", wxMEMORY_BITMAP(terrain_paint),
		"Paints the terrain", wxITEM_RADIO );

	map[Terrain_Raise] = this;
	map[Terrain_Paint] = this;
}

//-----------------------------------//

void TerrainMode::onTimer( wxTimerEvent& /*event*/ )
{
	Mouse* mouse = engine->getInputManager()->getMouse();
	const MouseInfo& info = mouse->getMouseInfo();

	if( info.leftButton )
	{
		if( op == nullptr )
		{
			debug( "Operation should exist" );
			return;
		}

		op->redo();
		editor->RefreshCanvas();
	}
}

//-----------------------------------//

void TerrainMode::onMouseDrag( const MouseDragEvent& mde )
{
	if( !mde.info.leftButton ) return;

	MouseButtonEvent mbe( mde.x, mde.y,
		MouseButton::Left, MouseEventType::MousePress );

	// We need to handle the case where the user clicks but
	// doesn't move the mouse anymore. As we don't receive
	// further events in this case, we need to check if the
	// mouse is still pressed.

	if( !op )
		createOperation();

	deformTerrain( mbe );
}

//-----------------------------------//

void TerrainMode::onMouseButtonPress( const MouseButtonEvent& mbe )
{
	if( !op ) 
		createOperation();

	deformTerrain( mbe );
}

//-----------------------------------//

void TerrainMode::onMouseButtonRelease( const MouseButtonEvent& )
{
	registerEvent();
}

//-----------------------------------//

void TerrainMode::onMouseLeave()
{
	// We use this event to check if the user leaves the window while
	// dragging in the middle of a terrain operation. If that isn't
	// the case, then we've got nothing to do here.
	if( !op ) return;
	
	registerEvent();
}

//-----------------------------------//

void TerrainMode::createOperation()
{
	if( op ) return;

	// If the left Shift is held down, then lower.
	Keyboard* kbd = engine->getInputManager()->getKeyboard();
	bool raise = !kbd->isKeyPressed( Keys::LShift );	
	
	TerrainTool::Enum tool = 
		raise ? TerrainTool::Raise : TerrainTool::Lower;

	op = new TerrainOperation( tool );
	op->size = 50;
	op->strength = 2;
}

//-----------------------------------//

void TerrainMode::registerEvent()
{
	timer.Stop();

	// Register the operation in the stack so it can be undone later.
	editor->registerOperation( op );

	op = nullptr;
}

//-----------------------------------//

void TerrainMode::deformTerrain( const MouseButtonEvent& mb )
{
	if( !pickTerrain( mb, op->res ) )
		return;

	// Perform operation!
	op->redo();
	editor->RefreshCanvas();
	
	timer.Start( TERRAIN_TIMER_MS );
}

//-----------------------------------//

bool TerrainMode::pickTerrain( const MouseButtonEvent& mb, 
							  RayTriangleQueryResult& res )
{
	const ScenePtr& scene = engine->getSceneManager();

	// Get a ray given the screen location clicked.
	const CameraPtr& camera = editor->viewport->camera;
	const Ray& pickRay = camera->getRay( mb.x, mb.y );

	RayBoxQueryResult query;
	if( !scene->doRayBoxQuery( pickRay, query ) )
		return false;

	const NodePtr& node = query.node;
	if( !node ) return false;

	const TerrainPtr& terrain = node->getComponent<Terrain>("Terrain");
	if( !terrain ) return false; // Picked nodes were not terrain.

	if( !scene->doRayTriangleQuery( pickRay, res, node ) )
		return false; // Query does not intersect the terrain.

	return true;
}

//-----------------------------------//

TerrainOperation::TerrainOperation( TerrainTool::Enum tool )
	: size( 0.0f ), strength( 0.0f ), savedState( false ),
	tool( tool )
{
	size = 50;
	strength = 2;
}

//-----------------------------------//

void TerrainOperation::undo()
{
	if( savedState )
	{
		processState( false );
		return;
	}
}

//-----------------------------------//

void TerrainOperation::redo()
{
	// If the operation can't be undone with an algorithm, 
	// then our best option is to save the state of the terrain.

	if( !savedState )
		processState( true );

	applyTerrainTool();
}

//-----------------------------------//

void TerrainOperation::processState( bool save )
{	
	RenderablePtr rend = res.renderable;
	if( !rend ) return;

	VertexBufferPtr vb = rend->getVertexBuffer();
	if( !vb ) return;

	if( save )
	{
		// No need to waste memory, let's save only what we need.
		foreach( const Vector3& v, vb->getVertices() )
			heights.push_back( v.y );

		savedState = true;
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
	if( !rend ) return;

	VertexBufferPtr vb = rend->getVertexBuffer();
	if( !vb ) return;

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
	{
		vb->forceRebuild();

		// TODO: pass bounding sphere to optimize normal calculation.
		//CellPtr cell = boost::static_pointer_cast<Cell>( rend );
		//cell->calculateNormals( vertices );
	}
}

//-----------------------------------//

} } // end namespaces