/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/scene/Geometry.h"
#include "vapor/scene/Node.h"

namespace vapor { namespace scene {

using namespace vapor::render;
using namespace vapor::math;

//-----------------------------------//

const std::string& Geometry::type = "Geometry";

//-----------------------------------//

Geometry::Geometry() : isDirty( true )
{

}

//-----------------------------------//

Geometry::Geometry( RenderablePtr rend )
	: isDirty( true )
{
	addRenderable( rend );
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
 
void Geometry::appendRenderables( render::RenderQueue& queue, TransformPtr transform )
{
	if( !transform ) return;
	
	const math::Matrix4x3& absoluteTransform = transform->getAbsoluteTransform();
	
	foreach( const rendPair& pair, renderables )
	{
		foreach( const RenderablePtr& rend, pair.second )
		{
			RenderState renderState;
			
			renderState.renderable = rend;
			renderState.modelMatrix = absoluteTransform;
			renderState.group = pair.first;
			renderState.priority = 0;

			queue.push_back( renderState );
		}
	}
}

//-----------------------------------//

void Geometry::update( double delta )
{
	if( !isDirty ) return;

	boundingVolume.reset();

	// Update the bounding box to accomodate new geometry.
	foreach( const RenderablePtr& rend, renderables[RenderGroup::Normal] )
	{
		const VertexBufferPtr& vb = rend->getVertexBuffer();
		if( !vb ) continue;

		foreach( const Vector3& v, vb->getVertices() )
			boundingVolume.add( v );
	}

	const NodePtr& node = getNode();
	node->getTransform()->markBoundingVolumeDirty();

	isDirty = false;
}

//-----------------------------------//

void Geometry::serialize( Json::Value& value )
{
	Component::serialize( value );
}

//-----------------------------------//

} } // end namespaces