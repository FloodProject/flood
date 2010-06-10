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

namespace vapor {

//-----------------------------------//

Cell::Cell( const TerrainSettings& settings, 
		   const std::vector<float>& heights, ushort x, ushort y ) 
	: Renderable( Primitive::Triangles ),
	settings( settings ),
	heights( heights ),
	x(x), y(y)
{
	updateGeometry();
}

//-----------------------------------//

void Cell::updateGeometry( )
{
	// Create a new VBO and upload triangle data
	VertexBufferPtr vb( new VertexBuffer() );
	IndexBufferPtr ib( new IndexBuffer() );

	// Vertex data
	std::vector<Vector3> vertex;
	std::vector<Vector3> colors;
	std::vector<Vector3> texCoords;
	std::vector<ushort> indices;

	const ushort CellSize = settings.CellSize;
	const ushort NumberTiles = settings.NumberTiles;
	const ushort MaxHeight = settings.MaxHeight;

	const float tileWidth = (float(CellSize) / NumberTiles);
	const float tileHeight = (float(CellSize) / NumberTiles);

	float dX = float(x * CellSize);
	float dZ = float(y * CellSize);
	
	float height;

	// Keeps track of the indices.
	ushort i = 0;

	// For each row of the cell
	for( short rowX = 0; rowX < NumberTiles; rowX++ )
	{
		// For each tile of the row
		for( short rowZ = 0; rowZ < NumberTiles; rowZ++ )
		{
			// Draw two triangles per tile.
			//
			//			1 3
			//			|/|
			//			2 4

			// Calculate the coordinates of the cell's triangles vertices.
			height = heights[(rowX)*(NumberTiles+1)+rowZ]*MaxHeight;
			Vector3 pt1( dX+(rowZ*tileWidth), height, dZ+rowX*tileHeight );
			
			height = heights[(rowX+1)*(NumberTiles+1)+rowZ]*MaxHeight;
			Vector3 pt2( dX+(rowZ*tileWidth), height, dZ+(rowX+1)*tileHeight );
			
			height = heights[(rowX)*(NumberTiles+1)+(rowZ+1)]*MaxHeight;
			Vector3 pt3( dX+(rowZ+1)*tileWidth, height, dZ+rowX*tileHeight );
			
			height = heights[(rowX+1)*(NumberTiles+1)+(rowZ+1)]*MaxHeight;
			Vector3 pt4( dX+(rowZ+1)*tileWidth, height, dZ+(rowX+1)*tileHeight );

			vertex.push_back( pt1 );
			vertex.push_back( pt2 );
			vertex.push_back( pt3 );
			vertex.push_back( pt4 );

			colors.push_back( Vector3( 1.0f, 1.0f, 1.0f ) );
			colors.push_back( Vector3( 1.0f, 1.0f, 1.0f ) );
			colors.push_back( Vector3( 1.0f, 1.0f, 1.0f ) );
			colors.push_back( Vector3( 1.0f, 1.0f, 1.0f ) );

			texCoords.push_back( Vector3( pt1.x / CellSize, pt1.z / CellSize, 0.0f ) );
			texCoords.push_back( Vector3( pt2.x / CellSize, pt2.z / CellSize, 0.0f ) );
			texCoords.push_back( Vector3( pt3.x / CellSize, pt3.z / CellSize, 0.0f ) );
			texCoords.push_back( Vector3( pt4.x / CellSize, pt4.z / CellSize, 0.0f ) );

			// First triangle
			indices.push_back( i) ;
			indices.push_back( i+1 );
			indices.push_back( i+2 );

			// Second triangle
			indices.push_back( i+2 ) ;
			indices.push_back( i+1 );
			indices.push_back( i+3 );

			i += 4;
		}
	}

	// Vertex buffer setup
	vb->set( VertexAttribute::Position, vertex );
	vb->set( VertexAttribute::Color, colors );
	vb->set( VertexAttribute::MultiTexCoord0, texCoords );
	setVertexBuffer( vb );

	ib->set( indices );
	setIndexBuffer( ib );

	// Update all the terrain normals.
	calculateNormals( vertex );
}


//-----------------------------------//

// Resources:
// http://www.gamedev.net/community/forums/topic.asp?topic_id=163625
// http://www.gamedev.net/reference/articles/article2264.asp
//
//#define tileToMat(x,y) ( (rowX)*(NumberTiles+1)+rowZ )
//#define matToTile(x,y) ( (rowX)*(NumberTiles+1)+rowZ )

#define N(i) n.push_back(i);

std::vector<uint> Cell::getNeighborVertices( uint i )
{
	const ushort NumberTiles = settings.NumberTiles;
	std::vector<uint> n;

	//  0-------4
	//  |/|/|/|/|
	//  5-------9
	//  |/|/|/|/|
	// 10-------14
	//  |/|/|/|/|
	// 15-------19
	//  |/|/|/|/|
	// 20-------24

	return n;
}

//-----------------------------------//

void Cell::calculateNormals( const std::vector<Vector3>& vs )
{
	if( !vb || !ib ) return;

	const std::vector<ushort>& ind = ib->getIndices16();

	// Calculate all face normals.
	std::vector<Vector3> faceNormals;

	for( uint i = 0; i < ind.size(); i += 3 )
	{
		Vector3 normal( calculateTriangleNormal(
			vs[ind[i]], vs[ind[i+1]], vs[ind[i+2]] ) );
		faceNormals.push_back( normal );
	}

	// Averaged per-vertex normals.
	std::vector<Vector3> normals;
	
	//uint i = 0;
	foreach( const Vector3& v, vs )
	{
	//	std::vector<int> ns = getNeighborVertices(i);

	//	Vector3 average = v;
	//	foreach( const Vector3& n, ns )
	//		average += n;

	//	average /= ns.size()+1;
	//	average.normalize();

		normals.push_back( Vector3::UnitY );
	//	normals.push_back( average );
	}

	vb->set( VertexAttribute::Normal, normals );
}

//-----------------------------------//

} // end namespace