/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"
#include "Render/Quad.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(Quad, Geometry)
REFLECT_CLASS_END()

//-----------------------------------//

Quad::Quad( float width, float height )
{
	RenderablePtr rend = RenderableCreate( AllocatorGetThis() );
	rend->setPrimitiveType(PolygonType::TriangleStrip);
	
	const VertexBufferPtr& vb =  createQuad(width, height);
	rend->setVertexBuffer(vb);

	MaterialHandle mat = MaterialCreate(AllocatorGetThis(), "Quad");
	mat.Resolve()->setBackfaceCulling(false);
	rend->setMaterial(mat);

	addRenderable(rend);
}

//-----------------------------------//

VertexBufferPtr Quad::createQuad( float width, float height )
{
	// Create a new VBO and upload triangle data
	VertexBufferPtr vb( Allocate(VertexBuffer, AllocatorGetHeap()) );

	// Vertex position data
	std::vector< Vector3 > vertex;
	vertex.push_back( Vector2(0.0f, 0.0f) );
	vertex.push_back( Vector2(width, 0.0f) );
	vertex.push_back( Vector2(0.0f, height) );
	vertex.push_back( Vector2(width, height) );

	// Vertex color data
	std::vector< Vector3 > colors;
	colors.push_back( Color::White );
	colors.push_back( Color::White );
	colors.push_back( Color::White );
	colors.push_back( Color::White );

	// Vertex tex coords data
	std::vector< Vector3 > coords;
	coords.push_back( Vector2(0.0f, 1.0f) );
	coords.push_back( Vector2(1.0f, 0.0f) );
	coords.push_back( Vector2(1.0f, 1.0f) );
	coords.push_back( Vector2(0.0f, 1.0f) );

	// Vertex buffer setup
	vb->set( VertexAttribute::Position, vertex );
	vb->set( VertexAttribute::Color, colors );
	vb->set( VertexAttribute::TexCoord0, coords );

	return vb;
}

//-----------------------------------//

NAMESPACE_ENGINE_END