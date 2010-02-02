/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/scene/Component.h"

namespace vapor { namespace scene {

//-----------------------------------//

Component::Component() : drawDebugRenderable( false )
{

}

//-----------------------------------//

Component::~Component()
{

}

//-----------------------------------//

void Component::setDebugRenderableVisible( bool draw )
{
	drawDebugRenderable = draw;
}

//-----------------------------------//

bool Component::isDebugRenderableVisible() const
{
	return drawDebugRenderable;
}

//-----------------------------------//

render::RenderablePtr Component::getDebugRenderable() const
{
	return render::RenderablePtr();
}

//-----------------------------------//

void Component::setNode( const NodePtr& node )
{
	this->node = node ;
}

//-----------------------------------//

NodePtr Component::getNode() const
{
	return node.lock();
}

//-----------------------------------//

} } // end namespaces