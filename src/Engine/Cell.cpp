/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#include "vapor/terrain/Cell.h"
#include "vapor/math/Color.h"

namespace vapor { namespace scene {

using namespace vapor::resources;
using namespace vapor::math;
using namespace vapor::render;

//-----------------------------------//

Cell::Cell( const TerrainSettings& settings, 
		   const std::vector<float>& heights, int x, int y ) 
	: Renderable( Primitive::Triangles ), heights( heights ),
	x( x ), y( y ), settings( settings )
{
	updateGeometry();
}

//-----------------------------------//

Cell::~Cell( )
{

}

//-----------------------------------//

void Cell::updateGeometry( )
{
	// Create a new VBO and upload triangle data
	VertexBufferPtr vb( new VertexBuffer() );

	// Vertex data
	std::vector< Vector3 > vertex;
	std::vector< Vector3 > colors;
	std::vector< Vector3 > texCoords;

	const short CellSize = settings.CellSize;
	const short TileDimensions = settings.TileDimensions;
	const short MaxHeight = settings.MaxHeight;

	const float tileWidth = (float(CellSize) / TileDimensions);
	const float tileHeight = (float(CellSize) / TileDimensions);

	float dX = x*CellSize;
	float dZ = y*CellSize;
	
	float height;

	// For each row of the Cell
	for( short rowX = 0; rowX < TileDimensions; rowX++ )
	{
		// For each cell of the row
		for( short rowZ = 0; rowZ < TileDimensions; rowZ++ )
		{
			// Draw two triangles per cell.
			//
			//			1 3
			//			|/|
			//			2 4

			// Calculate the coordinates of the cell's triangles vertices.
			height = heights[(rowX)*(TileDimensions+1)+rowZ]*MaxHeight;
			Vector3 pt1( dX+(rowZ*tileWidth), height, dZ+rowX*tileHeight );
			
			height = heights[(rowX+1)*(TileDimensions+1)+rowZ]*MaxHeight;
			Vector3 pt2( dX+(rowZ*tileWidth), height, dZ+(rowX+1)*tileHeight );
			
			height = heights[(rowX)*(TileDimensions+1)+(rowZ+1)]*MaxHeight;
			Vector3 pt3( dX+(rowZ+1)*tileWidth, height, dZ+rowX*tileHeight );
			
			height = heights[(rowX+1)*(TileDimensions+1)+(rowZ+1)]*MaxHeight;
			Vector3 pt4( dX+(rowZ+1)*tileWidth, height, dZ+(rowX+1)*tileHeight );

			// First triangle
			vertex.push_back( pt1 );
			vertex.push_back( pt2 );
			vertex.push_back( pt3 );

			colors.push_back( Vector3( 1.0f, 1.0f, 1.0f ) );
			colors.push_back( Vector3( 1.0f, 1.0f, 1.0f ) );
			colors.push_back( Vector3( 1.0f, 1.0f, 1.0f ) );

			// Second triangle
			vertex.push_back( pt2 );
			vertex.push_back( pt3 );
			vertex.push_back( pt4 );

			colors.push_back( Vector3( 1.0f, 1.0f, 1.0f ) );
			colors.push_back( Vector3( 1.0f, 1.0f, 1.0f ) );
			colors.push_back( Vector3( 1.0f, 1.0f, 1.0f ) );
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