/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/scene/Component.h"
#include "vapor/render/Renderable.h"

namespace vapor {

//-----------------------------------//

Component::Component()
	: drawDebugRenderable( false )
{ }

//-----------------------------------//

Component::~Component()
{ }

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

RenderablePtr Component::getDebugRenderable() const
{
	return RenderablePtr();
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

void Component::serialize( Json::Value& value )
{
	//value["type"] = getType();
	
	// No need to clutter the serialization with default values.
	if( drawDebugRenderable != false )
		value["drawDebugRenderable"] = drawDebugRenderable;
}

//-----------------------------------//

} // end namespace