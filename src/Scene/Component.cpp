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

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

REFLECT_ABSTRACT_CHILD_CLASS(Component, Object)
	FIELD_PRIMITIVE(0, bool, debugVisible)
REFLECT_CLASS_END()

//-----------------------------------//

Component::Component()
	: debugVisible(false)
	, debugInheritsTransform(true)
	, debugRenderable(nullptr)
	, entity(nullptr)
{
}

//-----------------------------------//

Component::~Component()
{
}

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
{
}

//-----------------------------------//

void Component::onDebugDraw( DebugDrawer&, DebugDrawFlags::Enum )
{
}

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

NAMESPACE_ENGINE_END