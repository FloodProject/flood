/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/terrain/Cell.h"
#include "vapor/terrain/Terrain.h"
#include "vapor/math/Math.h"
#include "vapor/math/Color.h"

namespace vapor { namespace scene {

using namespace vapor::resources;
using namespace vapor::math;
using namespace vapor::render;

//-----------------------------------//

Cell::Cell( const TerrainSettings& settings, 
		   const std::vector<float>& heights, ushort x, ushort y ) 
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
	std::vector< Vector3 > normals;
	std::vector< Vector3 > texCoords;

	const ushort CellSize = settings.CellSize;
	const ushort TileDimensions = settings.TileDimensions;
	const ushort MaxHeight = settings.MaxHeight;

	const float tileWidth = (float(CellSize) / TileDimensions);
	const float tileHeight = (float(CellSize) / TileDimensions);

	double dX = x * CellSize;
	double dZ = y * CellSize;
	
	float height;

	// Per-face normals. We'll use these to compute the proper averaged normals.
	std::vector< Vector3 > faceNormals;

	// For each row of the cell
	for( short rowX = 0; rowX < TileDimensions; rowX++ )
	{
		// For each tile of the row
		for( short rowZ = 0; rowZ < TileDimensions; rowZ++ )
		{
			// Draw two triangles per tile.
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

			faceNormals.push_back( calculateTriangleNormal( pt1, pt2, pt3 ) );

			texCoords.push_back( Vector3( pt1.x / CellSize, pt1.z / CellSize, 0.0f ) );
			texCoords.push_back( Vector3( pt2.x / CellSize, pt2.z / CellSize, 0.0f ) );
			texCoords.push_back( Vector3( pt3.x / CellSize, pt3.z / CellSize, 0.0f ) );

			// Second triangle
			vertex.push_back( pt3 );
			vertex.push_back( pt2 );
			vertex.push_back( pt4 );
			
			colors.push_back( Vector3( 1.0f, 1.0f, 1.0f ) );
			colors.push_back( Vector3( 1.0f, 1.0f, 1.0f ) );
			colors.push_back( Vector3( 1.0f, 1.0f, 1.0f ) );

			faceNormals.push_back( calculateTriangleNormal( pt3, pt2, pt4 ) );

			texCoords.push_back( Vector3( pt3.x / CellSize, pt3.z / CellSize, 0.0f ) );
			texCoords.push_back( Vector3( pt2.x / CellSize, pt2.z / CellSize, 0.0f ) );
			texCoords.push_back( Vector3( pt4.x / CellSize, pt4.z / CellSize, 0.0f ) );
		}
	}

	// Calculate averaged normals per-vertex.

	// For each row of the cell
	for( short rowX = 0; rowX < TileDimensions*3; rowX++ )
	{
		// For each tile of the row
		for( short rowZ = 0; rowZ < TileDimensions*2; rowZ++ )
		{
			//// Left and top-most vertex, just one face.
			//if( rowX == 0 && rowZ == 0 )
			//{
			//	normals.push_back( faceNormals[0] );
			//	continue;
			//}

			//// Left and top-most vertex, just one face.
			//if( rowX == 0 && rowZ == TileDimensions )
			//{
			//	normals.push_back( faceNormals[TileDimensions*2] );
			//	continue;
			//}

			//// Left-most vertex, average three faces.
			//if( rowX == 0 && rowZ < TileDimensions-1 )
			//{
			//	Vector3 average;
			//	
			//	int index = (2/*triangles per row*/*(rowZ)*TileDimensions);
			//	
			//	average += faceNormals[index];
			//	average += faceNormals[index+1];
			//	average += faceNormals[index+(2*TileDimensions)];

			//	normals.push_back( (average / 3).normalize() );
			//	continue;
			//}

			// Left-most vertex, average three faces.
			if( (rowX > 0) && (rowX < TileDimensions-1) && (rowZ > 0) && (rowZ < TileDimensions-1) )
			{
				Vector3 average;
				
				int index = (2/*triangles per row*/*(rowZ-1)*TileDimensions)+rowX;
				
				average += faceNormals[index-1];
				average += faceNormals[index];
				average += faceNormals[index+1];

				index = (2/*triangles per row*/*(rowZ+1)*TileDimensions)+rowX;

				average += faceNormals[index-1];
				average += faceNormals[index];
				average += faceNormals[index+1];

				normals.push_back( (average / 6).normalize() );
				continue;
			}

			//// Right-most vertex, average three faces.
			//if( rowX == TileDimensions-1 && rowZ < TileDimensions-1 )
			//{
			//	Vector3 average;
			//	
			//	int index = (2/*triangles per row*/*rowZ*TileDimensions);
			//	
			//	average += faceNormals[index];
			//	average += faceNormals[index+1];
			//	average += faceNormals[index+(2*TileDimensions)];

			//	normals.push_back( (average / 3).normalize() );
			//	continue;
			//}

			//// Left and top-most vertex, just one face.
			//if( rowX == TileDimensions-1 && rowZ == TileDimensions-1 )
			//{
			//	normals.push_back( faceNormals[faceNormals.size()-1] );
			//	continue;
			//}

			normals.push_back( Vector3::Zero );
		}
	}

	// Vertex buffer setup
	vb->set( VertexAttribute::Position, vertex );
	vb->set( VertexAttribute::Color, colors );
	vb->set( VertexAttribute::Normal, normals );
	vb->set( VertexAttribute::MultiTexCoord0, texCoords );

	setVertexBuffer( vb );
}

//-----------------------------------//

} } // end namespaces