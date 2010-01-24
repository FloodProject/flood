/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
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

Geometry::Geometry() 
{
}

//-----------------------------------//

Geometry::Geometry( RenderablePtr rend )
{
	addRenderable( rend );
}

//-----------------------------------//

Geometry::~Geometry()
{

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
	//TransformPtr transform = getNode()->getTransform();
	
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
}

//-----------------------------------//

void Geometry::update( double delta )
{

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