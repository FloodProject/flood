/************************************************************************
*
* Flood Project � (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Engine/API.h"

#include "Engine/Scene/Entity.h"
#include "Engine/Scene/Transform.h"
#include "Engine/Scene/Geometry.h"
#include "Engine/Scene/Group.h"
#include "Engine/Scene/Tags.h"
#include "Core/Utilities.h"
#include <algorithm>

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(Entity, Object)
	FIELD_PRIMITIVE(1, string, name)
	FIELD_PRIMITIVE(2, bool, visible)
	FIELD_VECTOR_PTR(3, Component, ComponentPtr, components, RefPointer)
	FIELD_PRIMITIVE(4, int32, tags)
	//FIELD_PRIMITIVE_CUSTOM(4, int32, tags, Bitfield)
REFLECT_CLASS_END()

//-----------------------------------//

Entity* EntityCreate(Allocator* alloc)
{
	return Allocate(alloc, Entity);
}

//-----------------------------------//

Entity::Entity()
	: visible(true)
	, tags(0)
	, parent(nullptr)
{
}

//-----------------------------------//

Entity::Entity( const String& name )
	: name(name)
	, visible(true)
	, tags(0)
	, parent(nullptr)
{
}

//-----------------------------------//

Entity::~Entity()
{
	// Keep a reference so it is the last component destroyed.
	TransformPtr transform = getTransform();
	
	components.clear();

	ComponentMap::iterator it = componentsMap.begin();
	
	for(; it != componentsMap.end(); it++ )
	{
		ComponentPtr& component = it->second;
		component.reset();
	}
}

//-----------------------------------//

static bool IsGroup(Entity* entity)
{
	return entity && Class::GetType(entity)->inherits(GroupGetType());
}

//-----------------------------------//

bool Entity::addComponent( const ComponentPtr& component )
{
	if( !component ) return false;

	Class* type = component->getType();

	if( componentsMap.find(type) != componentsMap.end() )
	{
		LogWarn( "Component '%s' already exists in '%s'", type->name, name.c_str() );
		return false;
	}

	componentsMap[type] = component;
	component->setEntity(this);

	onComponentAdded(component);
	sendEvents();

	if( IsGroup(parent) )
	{
		Group* group = (Group*) parent;
		group->onEntityComponentAdded(component);
	}

	components.push_back(component);

	return true;
}

//-----------------------------------//

bool Entity::removeComponent(const ComponentPtr& component)
{
	if (!component) return false;
	
	Class* type = component->getType();

	ComponentMap::iterator it = componentsMap.find(type);
	
	if (it == componentsMap.end())
		return false;

	componentsMap.erase(it);

	onComponentRemoved(component);
	sendEvents();

	if (IsGroup(parent))
	{
		Group* group = (Group*) parent;
		group->onEntityComponentRemoved(component);
	}

	if (type->inherits(GeometryGetType()))
		getTransform()->markBoundingVolumeDirty();

	return true;
}

//-----------------------------------//

ComponentPtr Entity::getComponent(const char* name) const
{
	Type* type = ReflectionDatabase::GetDatabase().findType(name);

	if (!type->isComposite())
		return nullptr;

	return getComponent((Class*) type);
}

//-----------------------------------//

ComponentPtr Entity::getComponent(Class* klass) const
{
	ComponentMap::const_iterator it = componentsMap.find(klass);
		
	if( it == componentsMap.end() )
		return nullptr;

	return it->second;
}

//-----------------------------------//

ComponentPtr Entity::getComponentFromFamily(Class* klass) const
{
	ComponentMap::const_iterator it;
	
	for( it = componentsMap.begin(); it != componentsMap.end(); it++ )
	{
		const ComponentPtr& component = it->second;
		Class* componentClass = component->getType();

		if (componentClass->inherits(klass))
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

		if( !component->getType()->inherits(GeometryGetType()) )
			continue;

		const GeometryPtr& geo = RefCast<Geometry>(component);
		geoms.push_back(geo);
	}

	return geoms;
}

//-----------------------------------//

void Entity::update( float delta )
{
	// Update all geometry bounding boxes first.
	const std::vector<GeometryPtr>& geoms = getGeometry();

	for( size_t i = 0; i < geoms.size(); i++ )
	{
		Geometry* geom = geoms[i].get();
		geom->update( delta );
	}

	// Update the transform component.
	Transform* transform = getTransform().get();
	if( transform ) transform->update( delta );

	// Update the other components.
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
	for(size_t i = 0; i < components.size(); i++ )
	{
		Component* component = components[i].get();
		if( !component ) continue;

		component->setEntity(this);
		
		Class* type = component->getType();
		componentsMap[type] = component;
	}

	if( !getTransform() ) addTransform();
}

//-----------------------------------//

void Entity::sendEvents()
{
	if( !parent ) return;

	Class* type =  parent->getType();

	if( !type->inherits(GroupGetType()) )
		return;

	Group* group = (Group*) parent;
	group->onEntityChanged();
}

//-----------------------------------//

bool Entity::addTransform()
{
	Transform* transform = TransformCreate( AllocatorGetThis() );
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

void Entity::setName( const String& name )
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

bool Entity::getTag(int32 index) const
{
	return GetBitFlag(tags, (uint32)(1 << index));
}

//-----------------------------------//

void Entity::setTag(int32 index, bool state)
{
	SetBitFlag(tags, (uint32)(1 << index), state);
}

//-----------------------------------//

NAMESPACE_ENGINE_END