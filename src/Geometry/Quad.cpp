/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"
#include "Geometry/Quad.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(Quad, Geometry)
REFLECT_CLASS_END()

//-----------------------------------//

Quad::Quad( float width, float height )
{
	const GeometryBufferPtr& gb = createQuad(width, height);

	MaterialHandle mat = MaterialCreate(AllocatorGetThis(), "Quad");
	mat.Resolve()->setBackfaceCulling(false);

	RenderablePtr rend = RenderBatchCreate( AllocatorGetThis() );
	rend->setPrimitiveType(PrimitiveType::TriangleStrip);
	rend->setGeometryBuffer(gb);
	rend->setMaterial(mat);

	addRenderable(rend);
}

//-----------------------------------//

GeometryBufferPtr Quad::createQuad( float width, float height )
{
	// Create a new buffer and store triangle data.
	GeometryBufferPtr gb = AllocateThis(GeometryBuffer);

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
	gb->set( VertexAttribute::Position, vertex );
	gb->set( VertexAttribute::Color, colors );
	gb->set( VertexAttribute::TexCoord0, coords );

	return gb;
}

//-----------------------------------//

NAMESPACE_ENGINE_END