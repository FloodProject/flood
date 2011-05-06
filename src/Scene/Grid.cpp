/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"
#include "Scene/Grid.h"

namespace vapor {

//-----------------------------------//

REFLECT_CHILD_CLASS(Grid, Geometry)
REFLECT_CLASS_END()

//-----------------------------------//

static const Vector3 LineColor(0.4f);
static const Vector3 MainLineColor(0.9f);
static const short   MainLineStep = 8;

//-----------------------------------//

Grid::Grid()
  : sizeX(1024)
  , sizeZ(1024)
  , divX(32)
  , divZ(32)
  , strongMainLines(true)
{ }

//-----------------------------------//

VertexBufferPtr Grid::buildGeometry()
{
	// Create a new VBO and upload triangle data
	VertexBufferPtr vb( Allocate(VertexBuffer, AllocatorGetHeap()) );

	// Vertex position data
	std::vector< Vector3 > vertex;

	// Vertex color data
	std::vector< Vector3 > colors;
	
	// Let's make the lines perpendicular to the X-axis.
	float x_pos = -sizeX / 2;
	float z_pos = -sizeZ / 2;	
	
	for( int i = 0; i < divX+1; i++ )
	{
		vertex.push_back( Vector3(x_pos, 0.0f, z_pos) );
		vertex.push_back( Vector3(-x_pos, 0.0f, z_pos) );

		bool isMainLine = (i % MainLineStep == 0) && (i != 0) && (i != divX);

		if( strongMainLines && isMainLine )
		{
			colors.push_back( MainLineColor );
			colors.push_back( MainLineColor );
		}
		else
		{
			colors.push_back( LineColor );
			colors.push_back( LineColor );
		}

		z_pos += sizeZ / divZ;
	}

	// Now the lines perpendicular to the Z-axis.
	x_pos = -sizeX / 2;
	z_pos = -sizeZ / 2;	
	
	for( int i = 0; i < divZ+1; i++ )
	{
		vertex.push_back( Vector3( x_pos, 0.0f, z_pos ) );
		vertex.push_back( Vector3( x_pos, 0.0f, -z_pos ) );

		bool isMainLine = (i % MainLineStep == 0) && (i != 0) && (i != divX);

		if( strongMainLines && isMainLine )
		{
			colors.push_back( MainLineColor );
			colors.push_back( MainLineColor );
		}
		else
		{
			colors.push_back( LineColor );
			colors.push_back( LineColor );
		}

		x_pos += sizeX / divX;
	}

	// Vertex buffer setup
	vb->set( VertexAttribute::Position, vertex );
	vb->set( VertexAttribute::Color, colors );

	return vb;
}

//-----------------------------------//

void Grid::update( float update )
{
	if( !renderables.empty() ) return;

	MaterialHandle materialHandle = MaterialCreate(AllocatorGetHeap(), "Grid");

	Material* material = materialHandle.Resolve();
	//material->setDepthTest(false);

	RenderablePtr rend = Allocate(Renderable, AllocatorGetHeap(), PolygonType::Lines);
	rend->setVertexBuffer( buildGeometry() );
	rend->setMaterial( materialHandle );

	addRenderable( rend );
}

//-----------------------------------//

} // end namespace