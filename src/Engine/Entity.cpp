/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "scene/Entity.h"
#include "scene/Transform.h"
#include "scene/Geometry.h"
#include "scene/Group.h"
#include "scene/Tags.h"
#include <algorithm>

namespace vapor {

//-----------------------------------//

BEGIN_CLASS(Entity)
	FIELD_PRIMITIVE(string, name)
	FIELD_PRIMITIVE(bool, visible)
	FIELD_PRIMITIVE_CUSTOM(int, tags, Bitfield)
END_CLASS()

//-----------------------------------//

Entity::Entity()
	: visible(true)
	, tags(0)
{ }

//-----------------------------------//


Entity::Entity( const std::string& name )
	: name(name)
	, visible(true)
	, tags(0)
{ }

//-----------------------------------//

bool Entity::addComponent( const ComponentPtr& component )
{
	if( !component )
		return false;

	const Class* type = &component->getType();

	if( components.find(type) != components.end() )
	{
		Log::warn( "Component of type '%s' already exists in entity '%s'", type->name.c_str(), name.c_str() );
		return false;
	}

	// Cache geometry components.
	if( type->is<Geometry>() || type->inherits<Geometry>() )
	{
		const GeometryPtr& geometry = std::static_pointer_cast<Geometry>( component );
		geometries.push_back( geometry );
	}

	// If it doesn't exist yet, add it in the components.
	components[type] = component;
	component->setEntity( shared_from_this() );

	onComponentAdded(component);
	sendEvents();

	return true;
}

//-----------------------------------//

bool Entity::removeComponent( const ComponentPtr& component )
{
	if( !component )
		return false;
	
	const Class* type = &component->getType();

	ComponentMap::iterator it = components.find(type);
	
	if( it == components.end() )
		return false;

	components.erase(it);
	
	if( !type->inherits<Geometry>() )
		return true;

	std::vector<GeometryPtr>::iterator it2;
	it2 = std::find( geometries.begin(), geometries.end(), component );
	
	assert( it2 != geometries.end() );
	geometries.erase( it2 );

	onComponentRemoved(component);
	sendEvents();

	return true;
}

//-----------------------------------//

void Entity::update( double delta )
{
	// Update all geometry bounding boxes first.
	const std::vector<GeometryPtr>& geoms = getGeometry();

	for( uint i = 0; i < geoms.size(); i++ )
	{
		const GeometryPtr& geom = geoms[i];
		geom->update( delta );
	}

	// Update the transform component.
	const TransformPtr& transform = getTransform();
	
	if( transform )
		transform->update( delta );

	// Update the other components.
	ComponentMap::const_iterator it;
	for( it = components.cbegin(); it != components.cend(); it++ )
	{
		const ComponentPtr& component = it->second;

		if( component == transform ) 
			continue;

		component->update( delta );
	}
}

//-----------------------------------//

void Entity::sendEvents()
{
	EntityPtr entity = parent.lock();

	if( !entity )
		return;

	const Class& type =  entity->getType();

	if( !type.is<Group>() && !type.inherits<Group>() )
		return;

	GroupPtr group = std::static_pointer_cast<Group>(entity);
	group->onEntityChanged();
}

//-----------------------------------//

bool Entity::addTransform()
{
	TransformPtr transform( new Transform() );
	return addComponent(transform);
}

//-----------------------------------//

TransformPtr Entity::getTransform() const
{
	return getComponent<Transform>();
}

//-----------------------------------//

EntityPtr Entity::getParent() const
{
	return parent.lock();
}

//-----------------------------------//

void Entity::setName( const std::string& name )
{
	this->name = name;
	sendEvents();
}

//-----------------------------------//

bool Entity::isVisible() const
{
	return visible;
}

//-----------------------------------//

bool Entity::getTag(int index)
{
	return (tags & index) ? true : false;
}

//-----------------------------------//

void Entity::setTag(int index, bool state)
{
	if(state)
		tags |= index;
	else
		tags &= ~index;
}

//-----------------------------------//

} // end namespace