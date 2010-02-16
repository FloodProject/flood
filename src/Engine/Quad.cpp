/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/render/Quad.h"

namespace vapor { namespace render {

using namespace vapor::math;

//-----------------------------------//

Quad::Quad( float width, float height )
	: Renderable( Primitive::Quads, getQuadBuffer( width, height ) )
{

}

//-----------------------------------//

render::VertexBufferPtr Quad::getQuadBuffer( float width, float height )
{
	// Create a new VBO and upload triangle data
	VertexBufferPtr vb( new VertexBuffer() );

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
	vb->set( VertexAttribute::Position, vertex );
	vb->set( VertexAttribute::Color, colors );
	vb->set( VertexAttribute::MultiTexCoord0, coords );

	return vb;
}

//-----------------------------------//

} } // end namespaces