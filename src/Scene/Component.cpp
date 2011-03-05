/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "Scene/Component.h"
#include "Render/Renderable.h"

namespace vapor {

//-----------------------------------//

BEGIN_CLASS_ABSTRACT(Component)
	//FIELD_PRIMITIVE(bool, debugVisible)
END_CLASS()

//-----------------------------------//

Component::Component()
	: debugVisible(false)
	, debugInheritsTransform(true)
{ }

//-----------------------------------//

Component::~Component()
{ }

//-----------------------------------//

Entity* Component::getEntity() const
{
	return entity;
}

//-----------------------------------//

void Component::setEntity( Entity* entity )
{
	this->entity = entity;
}

//-----------------------------------//

void Component::onPreRender( const Camera& camera )
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

} // end namespace