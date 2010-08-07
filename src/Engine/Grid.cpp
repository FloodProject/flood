/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/scene/Grid.h"

namespace vapor {

//-----------------------------------//

const std::string& Grid::type = "Grid";

static const Vector3 RegularLineColor(0.4f);
static const Vector3 StrongLineColor(0.9f);

//-----------------------------------//

Grid::Grid()
  : sizeX(1024)
  , sizeZ(1024)
  , divX(32)
  , divZ(32)
  , strongMainLines(true)
{
	MaterialPtr material( new Material("Grid") );
	
	RenderablePtr rend( new Renderable(Primitive::Lines) );
	rend->setVertexBuffer( buildGeometry() );
	rend->setMaterial( material );

	addRenderable( rend );
}

//-----------------------------------//

VertexBufferPtr Grid::buildGeometry()
{
	// Create a new VBO and upload triangle data
	VertexBufferPtr vb( new VertexBuffer() );

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

		bool isMainLine = (i % 8 == 0) && (i != 0) && (i != divX);

		if( strongMainLines && isMainLine )
		{
			colors.push_back( StrongLineColor );
			colors.push_back( StrongLineColor );
		}
		else
		{
			colors.push_back( RegularLineColor );
			colors.push_back( RegularLineColor );
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

		bool isMainLine = (i % 8 == 0) && (i != 0) && (i != divX);

		if( strongMainLines && isMainLine )
		{
			colors.push_back( StrongLineColor );
			colors.push_back( StrongLineColor );
		}
		else
		{
			colors.push_back( RegularLineColor );
			colors.push_back( RegularLineColor );
		}

		x_pos += sizeX / divX;
	}

	// Vertex buffer setup
	vb->set( VertexAttribute::Position, vertex );
	vb->set( VertexAttribute::Color, colors );

	return vb;
}

//-----------------------------------//

} // end namespace