/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Engine/API.h"

#include "Engine/Scene/Group.h"
#include "Engine/Scene/Transform.h"
#include "Core/Memory.h"
#include "Core/Containers/Array.h"

#include <algorithm>

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(Group, Entity)
	FIELD_VECTOR_PTR(6, Entity, EntityPtr, entities, RefPointer)
	FIELD_ALIAS(entities, "nodes")
REFLECT_CLASS_END()

//-----------------------------------//

Group::Group()
: entities(*AllocatorGetHeap())
{ }

//-----------------------------------//

Group::Group( const String& name )
	: Entity(name)
	, entities(*AllocatorGetHeap())
{ }

//-----------------------------------//

Group::~Group()
{
	for( size_t i = 0; i < array::size(entities); ++i )
	{
		const EntityPtr& entity = entities[i];
		
		if( entity->references != 1 )
			LogDebug("Entity '%s' should not have any references", entity->name.c_str());
	}
}

//-----------------------------------//

void Group::add( const EntityPtr& entity )
{
	if( !entity ) return;

	entity->setParent( this );
	array::push_back(entities, entity );

	onEntityAdded(entity);
	onEntityChanged();
}

//-----------------------------------//

EntityPtr Group::findEntity( const String& name ) const
{
	for( size_t i = 0; i < array::size(entities); ++i )
	{
		const EntityPtr& entity = entities[i];
		
		if( entity->getName() == name )
			return entity;
	}

	return nullptr;
}

//-----------------------------------//

bool Group::remove( const EntityPtr& entity )
{
	auto it = std::find(array::begin(entities), array::end(entities), entity);

	if( it == array::end(entities) )
		return false;

	onEntityRemoved(entity);
	onEntityChanged();

	array::remove(entities, it);

	return true;
}

//-----------------------------------//

void Group::update( float delta )
{
	for( size_t i = 0; i < array::size(entities); ++i )
	{
		const EntityPtr& entity = entities[i];
		if(entity) entity->update( delta );
	}
}

//-----------------------------------//

void Group::fixUp()
{
	Array<EntityPtr> invalid(*AllocatorGetHeap());

	for( size_t i = 0; i < array::size(entities); ++i )
	{
		const EntityPtr& entity = entities[i];
		
		if( !entity )
		{
			array::push_back(invalid, entity);
			continue;
		}

		if( !entity->getParent() ) entity->setParent(this);
	}

	// Remove invalid entities
	for( size_t i = 0; i < array::size(invalid); ++i )
	{
		auto it = std::find(array::begin(entities), array::end(entities), invalid[i]);
		array::remove(entities, it);
	}
}

//-----------------------------------//

NAMESPACE_ENGINE_END