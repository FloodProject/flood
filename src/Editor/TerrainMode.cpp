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

enum
{
	Terrain_Raise = 17226,
	Terrain_Paint
};

//-----------------------------------//

TerrainMode::TerrainMode( EditorFrame* frame )
	: Mode( frame )
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

void TerrainMode::deformTerrain( TerrainPtr terrain, 
								const RayTriangleQueryResult& res )
{
	RenderablePtr rend = res.renderable;
	if( !rend ) return;

	VertexBufferPtr vb = rend->getVertexBuffer();
	if( !vb ) return;

	const Vector3& pick = res.intersection;
	std::vector<Vector3> vertices = vb->getVertices();

	BoundingSphere bs( pick, 50 );
	foreach( Vector3& v, vertices )
	{
		if( !bs.intersects(v) )
			continue;

		//switch(terrainTool)
		//{
		//case Tool::Raise:
		//	raise();
		//case Tool::Lower:
		//	lower();
		//case Tool::Paint:
		//	paint();
		//case Tool::Level:
		//	level();
		//}
		v.y += 5;
	}

	vb->set( VertexAttribute::Position, vertices );

	// TODO: pass bounding sphere to optimize normal calculation.
	CellPtr cell = boost::static_pointer_cast<Cell>( rend );
	cell->calculateNormals( vertices );
}

//-----------------------------------//

void TerrainMode::onMouseClick(short x, short y)
{
	const ScenePtr& scene = engine->getSceneManager();

	// Get a ray given the screen location clicked.
	const Ray& pickRay = editor->viewport->camera->getRay( x, y );

	// Perform ray casting to find the nodes.
	RayBoxQueryList list;
	scene->doRayBoxQuery( pickRay, list );

	NodePtr node; TerrainPtr terrain;
	foreach( const RayBoxQueryResult& res, list )
	{
		node = res.node;

		// Search for nodes with terrains...
		terrain = node->getComponent<Terrain>("Terrain");
	
		if( terrain ) break;
	}

	// Picked nodes were not terrain, nothing to do...
	if( !node || !terrain ) return;

	RayTriangleQueryResult res;
	if( !scene->doRayTriangleQuery( pickRay, res, node ) )
		return;

	// We got a click on a terrain...
	deformTerrain( terrain, res );
}

//-----------------------------------//

} } // end namespaces