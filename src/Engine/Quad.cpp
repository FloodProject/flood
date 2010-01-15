/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#include "vapor/render/Quad.h"

namespace vapor { namespace render {

using namespace vapor::math;

//-----------------------------------//

Quad::Quad( const math::Dimension& dim, MaterialPtr mat, IndexBufferPtr ib )
	: Renderable( Primitive::Quads, getQuadBuffer( dim ), ib, mat )
{

}

//-----------------------------------//

Quad::~Quad()
{

}

//-----------------------------------//

render::VertexBufferPtr Quad::getQuadBuffer( const math::Dimension& dim )
{
	// Create a new VBO and upload triangle data
	VertexBufferPtr vb( new VertexBuffer() );

	const float width = static_cast< float >( dim.width );
	const float height = static_cast< float >( dim.height );

	// Vertex position data
	std::vector< Vector3 > vertex;
	vertex.push_back( Vector3( 0.0f, 0.0f, 0.0f ) );
	vertex.push_back( Vector3( 0.0f, height, 0.0f ) );
	vertex.push_back( Vector3( width, height, 0.0f ) );
	vertex.push_back( Vector3( width, 0.0f, 0.0f ) );

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