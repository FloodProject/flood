/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "Scene/Entity.h"
#include "Scene/Transform.h"
#include "Scene/Geometry.h"
#include "Scene/Group.h"
#include "Scene/Tags.h"
#include "Registry.h"
#include <algorithm>

namespace vapor {

//-----------------------------------//

BEGIN_CLASS(Entity)
	FIELD_PRIMITIVE(string, name)
	FIELD_PRIMITIVE(bool, visible)
	FIELD_VECTOR_PTR(Component, ComponentPtr, components, SharedPointer)
	FIELD_PRIMITIVE_CUSTOM(int, tags, Bitfield)
END_CLASS()

//-----------------------------------//

Entity::Entity()
	: visible(true)
	, tags(0)
	, parent(nullptr)
{ }

//-----------------------------------//


Entity::Entity( const std::string& name )
	: name(name)
	, visible(true)
	, tags(0)
	, parent(nullptr)
{ }

//-----------------------------------//

bool Entity::addComponent( const ComponentPtr& component )
{
	if( !component )
		return false;

	const Class* type = &component->getType();

	if( componentsMap.find(type) != componentsMap.end() )
	{
		Log::warn( "Component '%s' already exists in '%s'", type->name.c_str(), name.c_str() );
		return false;
	}

	componentsMap[type] = component;
	component->setEntity( this );

	onComponentAdded(component);
	sendEvents();

	components.push_back(component);

	return true;
}

//-----------------------------------//

bool Entity::removeComponent( const ComponentPtr& component )
{
	if( !component )
		return false;
	
	const Class* type = &component->getType();

	ComponentMap::iterator it = componentsMap.find(type);
	
	if( it == componentsMap.end() )
		return false;

	componentsMap.erase(it);

	onComponentRemoved(component);
	sendEvents();

	return true;
}

//-----------------------------------//

ComponentPtr Entity::getComponent(const std::string& name) const
{
	Registry& registry = Type::GetRegistry();
	const Type* type = registry.getType(name);

	if( !type )
		return nullptr;

	return getComponent(*type);
}

//-----------------------------------//

ComponentPtr Entity::getComponent(const Type& type) const
{
	if( !type.isClass() )
		return nullptr;

	const Class& klass = (Class&) type;

	ComponentMap::const_iterator it = componentsMap.find(&klass);
		
	if( it == componentsMap.end() )
		return nullptr;

	return it->second;
}

//-----------------------------------//

ComponentPtr Entity::getComponentFromFamily(const Type& type) const
{
	ComponentMap::const_iterator it;
	
	for( it = componentsMap.begin(); it != componentsMap.end(); it++ )
	{
		const ComponentPtr& component = it->second;
		const Type& componentType = component->getType();

		if( componentType.inherits(type) )
			return component;
	}

	return nullptr;
}

//-----------------------------------//

std::vector<GeometryPtr> Entity::getGeometry() const
{
	std::vector<GeometryPtr> geoms;

	ComponentMap::const_iterator it;
	for( it = componentsMap.begin(); it != componentsMap.end(); it++ )
	{
		const ComponentPtr& component = it->second;

		if( !component->getType().inherits<Geometry>() )
			continue;

		const GeometryPtr& geo = std::static_pointer_cast<Geometry>(component);
		geoms.push_back(geo);
	}

	return geoms;
}

//-----------------------------------//

void Entity::update( float delta )
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

	// Update the other componentsMap.
	ComponentMap::const_iterator it;
	for( it = componentsMap.begin(); it != componentsMap.end(); it++ )
	{
		const ComponentPtr& component = it->second;

		if( component == transform ) 
			continue;

		component->update( delta );
	}
}

//-----------------------------------//

void Entity::fixUp()
{
	for(uint i = 0; i < components.size(); i++ )
	{
		const ComponentPtr& component = components[i];
		
		component->setEntity( this );
		componentsMap[&component->getType()] = component;
	}
}

//-----------------------------------//

void Entity::sendEvents()
{
	if( !parent )
		return;

	const Class& type =  parent->getType();

	if( !type.is<Group>() && !type.inherits<Group>() )
		return;

	Group* group = (Group*) parent;
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

Entity* Entity::getParent() const
{
	return parent;
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

EntityPtr Entity::getShared()
{
	struct no_op_deleter
	{
		void operator()(void*) { }
	};

	return std::shared_ptr<Entity>(this, no_op_deleter());
	//return shared_from_this();
}

//-----------------------------------//

} // end namespace