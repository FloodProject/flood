/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Engine/API.h"

#include "Engine/Scene/Entity.h"
#include "Engine/Scene/Transform.h"
#include "Engine/Scene/Geometry.h"
#include "Engine/Scene/Group.h"
#include "Engine/Scene/Tags.h"
#include "Core/Containers/Hash.h"
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
	, components(*AllocatorGetHeap())
	, componentsMap(*AllocatorGetHeap())
{
}

//-----------------------------------//

Entity::Entity( const String& name )
	: name(name)
	, visible(true)
	, tags(0)
	, parent(nullptr)
	, components(*AllocatorGetHeap())
	, componentsMap(*AllocatorGetHeap())
{
}

//-----------------------------------//

Entity::~Entity()
{
	// Keep a reference so it is the last component destroyed.
	TransformPtr transform = getTransform();
	
	array::clear(components);

	for(auto c : componentsMap)
		c.value.reset();
}

//-----------------------------------//

static bool IsGroup(Entity* entity)
{
	return entity && ClassInherits(ClassGetType(entity), GroupGetType());
}

//-----------------------------------//

bool Entity::addComponent( const ComponentPtr& component )
{
	if( !component ) return false;

	Class* type = component->getType();

	if(hash::has(componentsMap, (uint64)type))
	{
		LogWarn( "Component '%s' already exists in '%s'", type->name, name.c_str() );
		return false;
	}

	hash::set(componentsMap, (uint64)type, component);
	component->setEntity(this);

	onComponentAdded(component);
	sendEvents();

	if( IsGroup(parent) )
	{
		Group* group = (Group*) parent;
		group->onEntityComponentAdded(component);
	}

	array::push_back(components, component);

	return true;
}

//-----------------------------------//

bool Entity::removeComponent( const ComponentPtr& component )
{
	if( !component ) return false;
	
	Class* type = component->getType();

	auto it = hash::get<ComponentPtr>(componentsMap, (uint64)type, nullptr);
	if(!it)
		return false;

	hash::remove(componentsMap, (uint64)type);
	onComponentRemoved(component);
	sendEvents();

	if( IsGroup(parent) )
	{
		Group* group = (Group*) parent;
		group->onEntityComponentRemoved(component);
	}

	if( ClassInherits(type, ReflectionGetType(Geometry)) )
		getTransform()->markBoundingVolumeDirty();

	return true;
}

//-----------------------------------//

ComponentPtr Entity::getComponent(const char* name) const
{
	Type* type = ReflectionFindType(name);

	if( !ReflectionIsComposite(type) )
		return nullptr;

	return getComponent((Class*) type);
}

//-----------------------------------//

ComponentPtr Entity::getComponent(Class* klass) const
{
	return hash::get<ComponentPtr>(componentsMap, (uint64)klass, nullptr);
}

//-----------------------------------//

ComponentPtr Entity::getComponentFromFamily(Class* klass) const
{
	for( auto it : componentsMap)
	{
		const auto& comp = it.value;
		Class* componentClass = comp->getType();

		if( ClassInherits(componentClass, klass) )
			return comp;
	}

	return nullptr;
}

//-----------------------------------//

Array<GeometryPtr> Entity::getGeometry() const
{
	Array<GeometryPtr> geoms(*AllocatorGetHeap());

	for( auto it : componentsMap)
	{
		const auto& component = it.value;

		if( !ClassInherits(component->getType(), ReflectionGetType(Geometry)) )
			continue;

		const GeometryPtr& geo = RefCast<Geometry>(component);
		array::push_back(geoms, geo);
	}

	return geoms;
}

//-----------------------------------//

void Entity::update( float delta )
{
	// Update all geometry bounding boxes first.
	const Array<GeometryPtr>& geoms = getGeometry();

	for( size_t i = 0; i < array::size(geoms); ++i )
	{
		Geometry* geom = geoms[i].get();
		geom->update( delta );
	}

	// Update the transform component.
	Transform* transform = getTransform().get();
	if( transform ) transform->update( delta );

	// Update the other components.
	for( auto it : componentsMap)
	{
		const auto& component = it.value;
		if( component == transform ) 
			continue;

		component->update( delta );
	}
}

//-----------------------------------//

void Entity::fixUp()
{
	for(size_t i = 0; i < array::size(components); ++i )
	{
		auto component = components[i];
		if( !component ) 
			continue;

		component->setEntity(this);
		hash::set(componentsMap, (uint64)component->getType(), component);
	}

	if( !getTransform() ) addTransform();
}

//-----------------------------------//

void Entity::sendEvents()
{
	if( !parent ) return;

	Class* type =  parent->getType();

	if( !ClassInherits(type, ReflectionGetType(Group)) )
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
	return GetBitFlag(tags, (1 << index));
}

//-----------------------------------//

void Entity::setTag(int32 index, bool state)
{
	SetBitFlag(tags, (1 << index), state);
}

//-----------------------------------//

NAMESPACE_ENGINE_END