/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "scene/Entity.h"
#include "scene/Tags.h"
#include "scene/Transform.h"
#include "scene/Geometry.h"

namespace vapor {

//-----------------------------------//

BEGIN_CLASS(Entity)
	FIELD_PRIMITIVE(Entity, string, name)
	FIELD_PRIMITIVE(Entity, bool, visible)
	FIELD_PRIMITIVE(Entity, Bitfield, tags)
END_CLASS()

//-----------------------------------//

Entity::Entity()
	: visible( true )
{ }

//-----------------------------------//


Entity::Entity( const std::string& name )
	: name( name )
	, visible( true )
{ }

//-----------------------------------//

bool Entity::addComponent( const ComponentPtr& component )
{
	if( !component )
		return false;

	const Class* type = &component->getInstanceType();

	if( components.find(type) != components.end() )
	{
		Log::warn( "Component of type '%s' already exists in entity '%s'", type->getName().c_str(), name.c_str() );
		return false;
	}

	// Cache geometry (renderable) objects.
	if( type->is<Geometry>() || type->inherits<Geometry>() )
	{
		const GeometryPtr& geometry = std::static_pointer_cast<Geometry>( component );
		geometries.push_back( geometry );
	}

	// If it doesn't exist yet, add it in the map.
	components[type] = component;
	component->setEntity( shared_from_this() );

	onComponentAdded(component);

	return true;
}

//-----------------------------------//

bool Entity::removeComponent( const ComponentPtr& component )
{
	if( !component )
		return false;
	
	const Class* type = &component->getInstanceType();

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

	return true;
}

//-----------------------------------//

void Entity::update( double delta )
{
	const TransformPtr& transform = getTransform();

	// Update all geometry bounding boxes first.
	for( uint i = 0; i < getGeometry().size(); i++ )
	{
		const GeometryPtr& geom = getGeometry()[i];
		geom->update( delta );
	}

	// Update transform (info may be needed by other components)
	if( transform )
		transform->update( delta );

	// Update everything else.
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

bool Entity::isVisible() const
{
	return visible;
}

//-----------------------------------//

bool Entity::getTag(int index)
{
	return tags[index];
}

//-----------------------------------//

void Entity::setTag(int index, bool state)
{
	tags[index] = state;
}

//-----------------------------------//

} // end namespace