/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#include "vapor/render/Quad.h"

namespace vapor {
	namespace render {

using namespace vapor::math;

//-----------------------------------//

Quad::Quad( MaterialPtr mat, IndexBufferPtr ib )
	: Renderable( Primitive::Quads, getQuadBuffer(), ib, mat )
{

}

//-----------------------------------//

Quad::~Quad()
{

}

//-----------------------------------//

render::VertexBufferPtr Quad::getQuadBuffer()
{
	// Create a new VBO and upload triangle data
	VertexBufferPtr vb( new VertexBuffer() );

	// Vertex position data
	std::vector< Vector3 > vertex;
	vertex.push_back( Vector3( -1.0f, 1.0f, 0.0f ) );
	vertex.push_back( Vector3( -1.0f, -1.0f, 0.0f ) );
	vertex.push_back( Vector3( 1.0f, -1.0f, 0.0f ) );
	vertex.push_back( Vector3( 1.0f, 1.0f, 0.0f ) );

	// Vertex color data
	std::vector< Vector3 > colors;
	colors.push_back( Vector3( 0.0f, 1.0f, 0.0f ) );
	colors.push_back( Vector3( 0.0f, 0.0f, 1.0f ) );
	colors.push_back( Vector3( 1.0f, 0.0f, 0.0f ) );
	colors.push_back( Vector3( 1.0f, 0.0f, 0.0f ) );

	// Vertex tex coords data
	std::vector< Vector3 > coords;
	coords.push_back( Vector3( 0.0f, 1.0f, 0.0f ) );
	coords.push_back( Vector3( 0.0f, 0.0f, 0.0f ) );
	coords.push_back( Vector3( 1.0f, 0.0f, 0.0f ) );
	coords.push_back( Vector3( 1.0f, 1.0f, 0.0f ) );

	// Vertex buffer setup
	vb->set( VertexAttribute::Vertex, vertex );
	vb->set( VertexAttribute::Color, colors );
	vb->set( VertexAttribute::MultiTexCoord0, coords );

	return vb;
}

//-----------------------------------//

} } // end namespaces