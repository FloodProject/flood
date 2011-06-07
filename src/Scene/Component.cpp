/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"
#include "Scene/Component.h"
#include "Render/Renderable.h"

namespace vapor {

//-----------------------------------//

REFLECT_ABSTRACT_CHILD_CLASS(Component, Object)
	FIELD_PRIMITIVE(bool, debugVisible)
REFLECT_CLASS_END()

//-----------------------------------//

Component::Component()
	: debugVisible(false)
	, debugInheritsTransform(true)
	, entity(nullptr)
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