/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Engine/API.h"
#include "Engine/Geometry/Cube.h"
#include "Graphics/RenderBatch.h"
#include "Core/Array.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(Cube, Geometry)
REFLECT_CLASS_END()

//-----------------------------------//

Cube::Cube()
	: width(1.0f), height(1.0f)
{
	create();
}

//-----------------------------------//

Cube::Cube( float width, float height )
	: width(width), height(height)
{
	create();
}

//-----------------------------------//

void Cube::create()
{
	GeometryBufferPtr gb = AllocateThis(GeometryBuffer);
	
	MaterialHandle mat = MaterialCreate(AllocatorGetHeap(), "Cube");
	mat.Resolve()->setShader("VertexColor");

	RenderablePtr rend = RenderBatchCreate( AllocatorGetHeap() );
	rend->setPrimitiveType(PrimitiveType::Quads);
	rend->setGeometryBuffer(gb);
	rend->setMaterial(mat);

	BuildCube(gb.get(), width, height);

	addRenderable(rend);
}

//-----------------------------------//

#define ADD_BOX_FACE( a, b, c, d )				\
	pos.pushBack( a*width );					\
	pos.pushBack( b*width );					\
	pos.pushBack( c*width );					\
	pos.pushBack( d*width );

#define v(a,b,c) Vector3(a,b,c)

void BuildCube( GeometryBuffer* gb, float width, float height )
{
	// Vertex position data
	Array<Vector3> pos;
	pos.reserve(24);

	ADD_BOX_FACE( v( 1, 1, 1), v(-1, 1, 1), v(-1,-1, 1), v( 1,-1, 1) )	// Front
	ADD_BOX_FACE( v( 1,-1, 1), v(-1,-1, 1), v(-1,-1,-1), v( 1,-1,-1) )	// Bottom
	ADD_BOX_FACE( v( 1,-1,-1), v(-1,-1,-1), v(-1, 1,-1), v( 1, 1,-1) )	// Back
	ADD_BOX_FACE( v( 1, 1,-1), v(-1, 1,-1), v(-1, 1, 1), v( 1, 1, 1) )	// Top
	ADD_BOX_FACE( v(-1, 1, 1), v(-1, 1,-1), v(-1,-1,-1), v(-1,-1, 1) )	// Left
	ADD_BOX_FACE( v( 1, 1,-1), v( 1, 1, 1), v( 1,-1, 1), v( 1,-1,-1) )	// Right

	// Vertex color data
	Array<Vector3> colors;
	colors.resize(pos.size());
	for(auto c : colors)
		c = Color::White;

	// Vertex tex coords data
	Array< Vector3 > coords;
	coords.reserve(pos.size());
	
	// Top
	coords.pushBack( Vector2(0.0f, 1.0f) );
	coords.pushBack( Vector2(0.0f, 0.0f) );
	coords.pushBack( Vector2(1.0f, 0.0f) );
	coords.pushBack( Vector2(1.0f, 1.0f) );
	// Bottom
	coords.pushBack( Vector2(1.0f, 1.0f) );
	coords.pushBack( Vector2(0.0f, 1.0f) );
	coords.pushBack( Vector2(0.0f, 0.0f) );
	coords.pushBack( Vector2(1.0f, 0.0f) );
	// Front
	coords.pushBack( Vector2(0.0f, 0.0f) );
	coords.pushBack( Vector2(1.0f, 0.0f) );
	coords.pushBack( Vector2(1.0f, 1.0f) );
	coords.pushBack( Vector2(0.0f, 1.0f) );
	// Back
	coords.pushBack( Vector2(1.0f, 0.0f) );
	coords.pushBack( Vector2(1.0f, 1.0f) );
	coords.pushBack( Vector2(0.0f, 1.0f) );
	coords.pushBack( Vector2(0.0f, 0.0f) );
	// Left
	coords.pushBack( Vector2(0.0f, 0.0f) );
	coords.pushBack( Vector2(1.0f, 0.0f) );
	coords.pushBack( Vector2(1.0f, 1.0f) );
	coords.pushBack( Vector2(0.0f, 1.0f) );
	// Right
	coords.pushBack( Vector2(0.0f, 0.0f) );
	coords.pushBack( Vector2(1.0f, 0.0f) );
	coords.pushBack( Vector2(1.0f, 1.0f) );
	coords.pushBack( Vector2(0.0f, 1.0f) );

	// Vertex buffer setup
	gb->set( VertexAttribute::Position, pos );
	gb->set( VertexAttribute::Color, colors );
	gb->set( VertexAttribute::TexCoord0, coords );
}

//-----------------------------------//

NAMESPACE_ENGINE_END