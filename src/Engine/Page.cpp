/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/terrain/Page.h"

namespace vapor { namespace scene {

using namespace vapor::resources;
using namespace vapor::math;
using namespace vapor::render;

//-----------------------------------//

// Page size in units.
static const short PageWidth = 500;
static const short PageHeight = 500;

// Number of cells in the X and Z axis.
static const short CellsPerRow = 10;
static const short CellsPerColumn = 10;

//-----------------------------------//

Page::Page( int x, int y ) 
	: Renderable( Primitive::TriangleStrip ), x( x ), y( y )
{
	updateGeometry();
}

//-----------------------------------//

Page::~Page( )
{

}

//-----------------------------------//

void Page::updateGeometry( )
{
	// Create a new VBO and upload triangle data
	VertexBufferPtr vb( new VertexBuffer() );

	// Vertex data
	std::vector< Vector3 > vertex;
	std::vector< Vector3 > colors;
	std::vector< Vector3 > texCoords;

	short Height;

	const short cellWidth = (PageWidth / CellsPerRow);
	const short cellHeight = (PageHeight / CellsPerColumn);

	// For each row of the page
	for( short rowX = 0; rowX < CellsPerColumn; rowX++ )
	{
		// For each cell of the row
		for( short rowZ = 0; rowZ < CellsPerRow; rowZ++ )
		{
			// Draw two triangles per cell.
			//
			//			1 3
			//			|/|
			//			2 4

			float height = 0.0f;
			float dX = x*PageWidth;
			float dZ = y*PageHeight;
			
			Vector3 pt1( dX+rowX*cellWidth, height, dZ+rowZ*cellHeight );
			Vector3 pt2( dX+rowX*cellWidth, height, dZ+(rowZ+1)*cellHeight );
			Vector3 pt3( dX+(rowX+1)*cellWidth, height, dZ+rowZ*cellHeight );
			Vector3 pt4( dX+(rowX+1)*cellWidth, height, dZ+(rowZ+1)*cellHeight );

			// First triangle
			vertex.push_back( pt1 );
			vertex.push_back( pt2 );
			vertex.push_back( pt3 );

			colors.push_back( Vector3( 0.0f, 0.0f, 0.0f ) );
			colors.push_back( Vector3( 0.0f, 0.0f, 0.0f ) );
			colors.push_back( Vector3( 0.0f, 0.0f, 0.0f ) );

			// Second triangle
			vertex.push_back( pt2 );
			vertex.push_back( pt3 );
			vertex.push_back( pt4 );

			colors.push_back( Vector3( 0.0f, 0.0f, 0.0f ) );
			colors.push_back( Vector3( 0.0f, 0.0f, 0.0f ) );
			colors.push_back( Vector3( 0.0f, 0.0f, 0.0f ) );
		}
	}

	// Vertex buffer setup
	vb->set( VertexAttribute::Vertex, vertex );
	vb->set( VertexAttribute::Color, colors );
	//vb->set( VertexAttribute::MultiTexCoord0, texCoords );

	setVertexBuffer( vb );
}

//-----------------------------------//

} } // end namespaces