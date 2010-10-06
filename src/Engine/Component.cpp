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

BEGIN_CLASS_ABSTRACT(Component)
	FIELD_PRIMITIVE(Component, bool, debugVisible)
END_CLASS()

//-----------------------------------//

Component::Component()
	: debugVisible( false )
{ }

//-----------------------------------//

Component::~Component()
{ }

//-----------------------------------//

void Component::setDebugRenderableVisible( bool visible )
{
	debugVisible = visible;
}

//-----------------------------------//

bool Component::isDebugRenderableVisible() const
{
	return debugVisible;
}

//-----------------------------------//

RenderablePtr Component::getDebugRenderable() const
{
	if( !debugRenderable )
		debugRenderable = createDebugRenderable();

	return debugRenderable;
}

//-----------------------------------//

RenderablePtr Component::createDebugRenderable() const
{
	return nullptr;
}

//-----------------------------------//

void Component::setNode( const NodePtr& newNode )
{
	node = newNode;
}

//-----------------------------------//

NodePtr Component::getNode() const
{
	return node.lock();
}

//-----------------------------------//

} // end namespace