/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/PCH.h"
#include "vapor/render/DebugGeometry.h"

namespace vapor { namespace render {

using namespace vapor::math;

//-----------------------------------//

static const float EXTRA_SPACE = 1.01f;

#define ADD_BOX_FACE( a, b, c, d )						\
	v.push_back( aabb.getCorner( a ) * EXTRA_SPACE );	\
	v.push_back( aabb.getCorner( b ) * EXTRA_SPACE );	\
	v.push_back( aabb.getCorner( c ) * EXTRA_SPACE );	\
	v.push_back( aabb.getCorner( d ) * EXTRA_SPACE );

RenderablePtr buildBoundingRenderable( const math::AABB& aabb )
{
	VertexBufferPtr vb( new VertexBuffer() );

	std::vector< Vector3 > v;
	ADD_BOX_FACE( 0, 2, 3, 1 ) // Front
	ADD_BOX_FACE( 0, 1, 5, 4 ) // Bottom
	ADD_BOX_FACE( 4, 5, 7, 6 ) // Back
	ADD_BOX_FACE( 2, 6, 7, 3 ) // Top
	ADD_BOX_FACE( 0, 4, 6, 2 ) // Left
	ADD_BOX_FACE( 1, 3, 7, 5 ) // Right

	std::vector< Vector3 > c( 6/*faces*/*4/*vertices*/, Vector3( 1.0f, 1.0f, 0.0f ) );
	vb->set( VertexAttribute::Position, v );
	vb->set( VertexAttribute::Color, c );

	MaterialPtr mat( new Material( "BoundBox", "diffuse" ) );
	mat->setLineWidth( 1.0f );
	mat->setLineSmoothing( true );
	mat->setBackfaceCulling( false );

	RenderablePtr bbox( new Renderable( Primitive::Quads, vb, mat ) );
	bbox->setPolygonMode( PolygonMode::Wireframe );

	return bbox;
}

//-----------------------------------//

} } // end namespaces