/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "render/Cube.h"

namespace vapor {

//-----------------------------------//

Cube::Cube( float width, float height )
	: Renderable( PolygonType::Quads )
{
	VertexBufferPtr vb = buildGeometry(width, height);
	setVertexBuffer(vb);
}

//-----------------------------------//

#define ADD_BOX_FACE( a, b, c, d )				\
	pos.push_back( a*width );					\
	pos.push_back( b*width );					\
	pos.push_back( c*width );					\
	pos.push_back( d*width );

#define v(a,b,c) Vector3(a,b,c)

VertexBufferPtr Cube::buildGeometry( float width, float height )
{
	// Create a new VBO and upload triangle data
	VertexBufferPtr vb( new VertexBuffer() );

	// Vertex position data
	std::vector<Vector3> pos;
	ADD_BOX_FACE( v(1 , 1,-1), v(-1, 1,-1), v(-1, 1, 1), v( 1, 1, 1) )	// Top
	ADD_BOX_FACE( v(1 ,-1, 1), v(-1,-1, 1), v(-1,-1,-1), v( 1,-1,-1) )	// Bottom
	ADD_BOX_FACE( v(1 , 1, 1), v(-1, 1, 1), v(-1,-1, 1), v( 1,-1, 1) )	// Front
	ADD_BOX_FACE( v(1 ,-1,-1), v(-1,-1,-1), v(-1, 1,-1), v( 1, 1,-1) )	// Back
	ADD_BOX_FACE( v(-1, 1, 1), v(-1, 1,-1), v(-1,-1,-1), v(-1,-1, 1) )	// Left
	ADD_BOX_FACE( v(1 , 1,-1), v( 1, 1, 1), v( 1,-1, 1), v( 1,-1,-1) )	// Right

	// Vertex color data
	//std::vector< Vector3 > colors;
	//colors.push_back( Color::White );
	//colors.push_back( Color::White );
	//colors.push_back( Color::White );
	//colors.push_back( Color::White );

	// Vertex tex coords data
	std::vector< Vector3 > coords;
	
	//// Top
	//coords.push_back( Vector2(0.0f, 1.0f) );
	//coords.push_back( Vector2(0.0f, 0.0f) );
	//coords.push_back( Vector2(1.0f, 0.0f) );
	//coords.push_back( Vector2(1.0f, 1.0f) );
	//// Bottom
	//coords.push_back( Vector2(1.0f, 1.0f) );
	//coords.push_back( Vector2(0.0f, 1.0f) );
	//coords.push_back( Vector2(0.0f, 0.0f) );
	//coords.push_back( Vector2(1.0f, 0.0f) );
	//// Front
	//coords.push_back( Vector2(0.0f, 0.0f) );
	//coords.push_back( Vector2(1.0f, 0.0f) );
	//coords.push_back( Vector2(1.0f, 1.0f) );
	//coords.push_back( Vector2(0.0f, 1.0f) );
	//// Back
	//coords.push_back( Vector2(1.0f, 0.0f) );
	//coords.push_back( Vector2(1.0f, 1.0f) );
	//coords.push_back( Vector2(0.0f, 1.0f) );
	//coords.push_back( Vector2(0.0f, 0.0f) );
	//// Left
	//coords.push_back( Vector2(0.0f, 0.0f) );
	//coords.push_back( Vector2(1.0f, 0.0f) );
	//coords.push_back( Vector2(1.0f, 1.0f) );
	//coords.push_back( Vector2(0.0f, 1.0f) );
	//// Right
	//coords.push_back( Vector2(0.0f, 0.0f) );
	//coords.push_back( Vector2(1.0f, 0.0f) );
	//coords.push_back( Vector2(1.0f, 1.0f) );
	//coords.push_back( Vector2(0.0f, 1.0f) );

	// Vertex buffer setup
	vb->set( VertexAttribute::Position, pos );
	//vb->set( VertexAttribute::Color, colors );
	//vb->set( VertexAttribute::TexCoord0, coords );

	return vb;
}

//-----------------------------------//

} // end namespace