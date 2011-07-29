/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"
#include "Render/Cube.h"
#include "Render/Renderable.h"

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
	VertexBufferPtr vb = Allocate(VertexBuffer, AllocatorGetHeap());
	
	MaterialHandle mat = MaterialCreate(AllocatorGetHeap(), "Cube");
	mat.Resolve()->setProgram("VertexColor");

	RenderablePtr rend = RenderableCreate( AllocatorGetHeap() );
	rend->setPrimitiveType(PolygonType::Quads);
	rend->setVertexBuffer(vb);
	rend->setMaterial(mat);

	BuildCube(vb.get(), width, height);

	addRenderable(rend);
}

//-----------------------------------//

#define ADD_BOX_FACE( a, b, c, d )				\
	pos.push_back( a*width );					\
	pos.push_back( b*width );					\
	pos.push_back( c*width );					\
	pos.push_back( d*width );

#define v(a,b,c) Vector3(a,b,c)

void BuildCube( VertexBuffer* vb, float width, float height )
{
	// Vertex position data
	std::vector<Vector3> pos;
	pos.reserve(24);

	ADD_BOX_FACE( v( 1, 1, 1), v(-1, 1, 1), v(-1,-1, 1), v( 1,-1, 1) )	// Front
	ADD_BOX_FACE( v( 1,-1, 1), v(-1,-1, 1), v(-1,-1,-1), v( 1,-1,-1) )	// Bottom
	ADD_BOX_FACE( v( 1,-1,-1), v(-1,-1,-1), v(-1, 1,-1), v( 1, 1,-1) )	// Back
	ADD_BOX_FACE( v( 1, 1,-1), v(-1, 1,-1), v(-1, 1, 1), v( 1, 1, 1) )	// Top
	ADD_BOX_FACE( v(-1, 1, 1), v(-1, 1,-1), v(-1,-1,-1), v(-1,-1, 1) )	// Left
	ADD_BOX_FACE( v( 1, 1,-1), v( 1, 1, 1), v( 1,-1, 1), v( 1,-1,-1) )	// Right

	// Vertex color data
	std::vector<Vector3> colors( pos.size(), Color::White );

	// Vertex tex coords data
	std::vector< Vector3 > coords( pos.size(), Vector3::Zero );
	
#if 0
	// Top
	coords.push_back( Vector2(0.0f, 1.0f) );
	coords.push_back( Vector2(0.0f, 0.0f) );
	coords.push_back( Vector2(1.0f, 0.0f) );
	coords.push_back( Vector2(1.0f, 1.0f) );
	// Bottom
	coords.push_back( Vector2(1.0f, 1.0f) );
	coords.push_back( Vector2(0.0f, 1.0f) );
	coords.push_back( Vector2(0.0f, 0.0f) );
	coords.push_back( Vector2(1.0f, 0.0f) );
	// Front
	coords.push_back( Vector2(0.0f, 0.0f) );
	coords.push_back( Vector2(1.0f, 0.0f) );
	coords.push_back( Vector2(1.0f, 1.0f) );
	coords.push_back( Vector2(0.0f, 1.0f) );
	// Back
	coords.push_back( Vector2(1.0f, 0.0f) );
	coords.push_back( Vector2(1.0f, 1.0f) );
	coords.push_back( Vector2(0.0f, 1.0f) );
	coords.push_back( Vector2(0.0f, 0.0f) );
	// Left
	coords.push_back( Vector2(0.0f, 0.0f) );
	coords.push_back( Vector2(1.0f, 0.0f) );
	coords.push_back( Vector2(1.0f, 1.0f) );
	coords.push_back( Vector2(0.0f, 1.0f) );
	// Right
	coords.push_back( Vector2(0.0f, 0.0f) );
	coords.push_back( Vector2(1.0f, 0.0f) );
	coords.push_back( Vector2(1.0f, 1.0f) );
	coords.push_back( Vector2(0.0f, 1.0f) );
#endif

	// Vertex buffer setup
	vb->set( VertexAttribute::Position, pos );
	vb->set( VertexAttribute::Color, colors );
	vb->set( VertexAttribute::TexCoord0, coords );
}

//-----------------------------------//

NAMESPACE_ENGINE_END