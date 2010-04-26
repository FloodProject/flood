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
#include "vapor/render/Renderable.h"
#include "vapor/scene/Node.h"
#include "vapor/scene/Geometry.h"
#include "vapor/scene/Tags.h"

namespace vapor { namespace render {

using namespace vapor::math;
using namespace vapor::scene;

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
	mat->setDepthTest( false );

	RenderablePtr bbox( new Renderable(Primitive::Quads, vb, mat) );
	bbox->setPolygonMode( PolygonMode::Wireframe );

	return bbox;
}

//-----------------------------------//

NodePtr buildRay( const Ray& pickRay, const Vector3& outFar )
{
	std::vector< Vector3 > vertex;
	vertex.push_back( pickRay.origin );
	vertex.push_back( outFar );

	std::vector< Vector3 > colors;
	colors.push_back( Vector3(1.0f, 0.0f, 0.0f) );
	colors.push_back( Vector3(1.0f, 0.0f, 0.0f) );

	VertexBufferPtr vb( new VertexBuffer() );
	vb->set( VertexAttribute::Position, vertex );
	vb->set( VertexAttribute::Color, colors );

	MaterialPtr mat( new Material("LineMaterial") );
	
	RenderablePtr rend( new Renderable(Primitive::Lines, vb) );
	rend->setMaterial( mat );	
	
	GeometryPtr geom( new Geometry(rend) );
	
	NodePtr line( new Node("line") );
	line->addTransform();
	line->addComponent( geom );
	line->setTag( Tags::NonPickable, true );
	
	return line;
}

//-----------------------------------//

} } // end namespaces