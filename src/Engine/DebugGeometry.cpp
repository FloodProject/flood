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
#include "vapor/scene/Tags.h"

namespace vapor {

//-----------------------------------//

#define ADD_BOX_FACE( a, b, c, d )		\
	pos.push_back( box.getCorner(a) );	\
	pos.push_back( box.getCorner(b) );	\
	pos.push_back( box.getCorner(c) );	\
	pos.push_back( box.getCorner(d) );

RenderablePtr buildBoundingRenderable( const BoundingBox& box )
{
	std::vector<Vector3> pos;
	ADD_BOX_FACE( 0, 2, 3, 1 ) // Front
	ADD_BOX_FACE( 0, 1, 5, 4 ) // Bottom
	ADD_BOX_FACE( 4, 5, 7, 6 ) // Back
	ADD_BOX_FACE( 2, 6, 7, 3 ) // Top
	ADD_BOX_FACE( 0, 4, 6, 2 ) // Left
	ADD_BOX_FACE( 1, 3, 7, 5 ) // Right

	const int numColors = 6*4; // Faces*Vertices
	std::vector< Vector3 > colors( numColors, Color::White );

	VertexBufferPtr vb( new VertexBuffer() );
	vb->set( VertexAttribute::Position, pos );
	vb->set( VertexAttribute::Color, colors );

	MaterialPtr mat( new Material("BoundingBox", "Diffuse") );
	mat->setBackfaceCulling( false );

	RenderablePtr boundingBox( new Renderable(PolygonType::Quads, vb, mat) );
	boundingBox->setPolygonMode( PolygonMode::Wireframe );

	return boundingBox;
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

	MaterialPtr mat = new Material("LineMaterial");
	RenderablePtr rend = new Renderable(PolygonType::Lines, vb, mat);
	GeometryPtr geom( new Geometry(rend) );
	
	NodePtr line( new Node("Line") );
	line->setTag( Tags::NonPickable, true );
	line->addTransform();
	line->addComponent( geom );
	
	return line;
}

//-----------------------------------//

} // end namespace