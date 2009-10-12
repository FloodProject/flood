/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#include "vapor/scene/Geometry.h"

using namespace vapor::render;

namespace vapor {
	namespace scene {

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

const std::vector< render::RenderablePtr >& Geometry::getRenderables( RenderGroup::Enum group )
{ 
	return renderables[group]; 
}

//-----------------------------------//
 
void Geometry::appendRenderables( render::RenderQueue& queue )
{
	foreach( const rendPair& pair, renderables )
	{
		foreach( RenderablePtr rend, pair.second )
		{
			RenderState renderState;
			
			renderState.renderable = rend;
			renderState.modelMatrix = this->getAbsoluteTransform();
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

const std::string Geometry::name() const
{
	return "Geometry";
}

//-----------------------------------//

} } // end namespaces