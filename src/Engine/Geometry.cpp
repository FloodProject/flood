/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#include "vapor/scene/Geometry.h"
#include "vapor/scene/Node.h"

using namespace vapor::render;
using namespace vapor::math;

namespace vapor {
	namespace scene {

//-----------------------------------//

const std::string& Geometry::type = "Geometry";

//-----------------------------------//

Geometry::Geometry() 
	: isDirty( true ), drawBoundingBox( false )
{
	buildBoundingRenderable();
}

//-----------------------------------//

Geometry::Geometry( RenderablePtr rend )
	: isDirty( true ), drawBoundingBox( false )
{
	addRenderable( rend );
	buildBoundingRenderable();
}

//-----------------------------------//

Geometry::~Geometry()
{

}

//-----------------------------------//

void Geometry::buildBoundingRenderable()
{
	VertexBufferPtr vb( new VertexBuffer() );
	bbox.reset( new Renderable( Primitive::Lines, vb ) );
}

//-----------------------------------//

void Geometry::addRenderable( RenderablePtr rend, 
							 RenderGroup::Enum group, 
							 uint priority )
{
	renderables[group].push_back( rend );
}

//-----------------------------------//

const std::vector< render::RenderablePtr >& 
Geometry::getRenderables( RenderGroup::Enum group )
{ 
	return renderables[group]; 
}

//-----------------------------------//
 
void Geometry::appendRenderables( render::RenderQueue& queue )
{
	TransformPtr transform = getNode()->getTransform();
	
	// The node should have a transform component.
	assert( transform != nullptr );

	if( !transform ) return;
	
	const math::Matrix4x3& absoluteTransform = transform->getAbsoluteTransform();
	
	foreach( const rendPair& pair, renderables )
	{
		foreach( RenderablePtr rend, pair.second )
		{
			RenderState renderState;
			
			renderState.renderable = rend;
			renderState.modelMatrix = absoluteTransform;
			renderState.group = pair.first;
			renderState.priority = 0;

			queue.push_back( renderState );
		}
	}

	if( drawBoundingBox )
	{
		RenderState renderState;
		
		renderState.renderable = bbox;
		renderState.modelMatrix = absoluteTransform;
		renderState.group = RenderGroup::Normal;
		renderState.priority = 0;

		queue.push_back( renderState );
	}
}

//-----------------------------------//

const math::AABB& Geometry::getBoundingVolume() const
{
	return boundingVolume;
}

//-----------------------------------//

void Geometry::update( float delta )
{
	if( !isDirty ) return;

	boundingVolume.reset();

	// Update the bounding box to accomodate new geometry.
	foreach( RenderablePtr rend, renderables[RenderGroup::Normal] )
	{
		std::vector<math::Vector3> vertices = rend->getVertexBuffer()->getVertices();
		
		foreach( const Vector3& v, vertices )
			boundingVolume.add( v );
	}

	VertexBufferPtr vb = bbox->getVertexBuffer();
	std::vector< Vector3 > vertices;
	for( int i = 0; i < 8; i++ )
	{
		vertices.push_back( boundingVolume.getCorner( i ) );
	}
	vb->set( VertexAttribute::Vertex, vertices );
}

//-----------------------------------//

void Geometry::setBoundingBoxVisible( bool visible )
{
	drawBoundingBox = visible;
}

//-----------------------------------//

const std::string Geometry::save(int indent)
{
	return "";
}

//-----------------------------------//

const std::string& Geometry::getType() const
{
	return type;
}

//-----------------------------------//

} } // end namespaces