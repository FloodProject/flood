/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"
#include "Scene/Group.h"
#include <algorithm>

namespace vapor {

//-----------------------------------//

REFLECT_CHILD_CLASS(Group, Entity)
	FIELD_VECTOR_PTR(Entity, EntityPtr, entities, RefPointer)
	FIELD_ALIAS(entities, "nodes")
REFLECT_CLASS_END()

//-----------------------------------//

Group::Group()
{ }

//-----------------------------------//

Group::Group( const String& name )
	: Entity(name)
{ }

//-----------------------------------//

void Group::add( const EntityPtr& entity )
{
	if( !entity ) return;

	entity->setParent( this );
	entities.push_back( entity );

	onEntityAdded(entity);
	onEntityChanged();
}

//-----------------------------------//

EntityPtr Group::findEntity( const std::string& name ) const
{
	for( size_t i = 0; i < entities.size(); i++ )
	{
		const EntityPtr& entity = entities[i];
		
		if( entity->getName() == name )
			return entity;
	}

	return EntityPtr();
}

//-----------------------------------//

bool Group::remove( const EntityPtr& entity )
{
	std::vector<EntityPtr>::iterator it;
	it = std::find(entities.begin(), entities.end(), entity);

	if( it == entities.end() )
		return false;

	onEntityRemoved(entity);
	onEntityChanged();

	entities.erase(it);

	return true;
}

//-----------------------------------//

void Group::update( float delta )
{
	for( size_t i = 0; i < entities.size(); i++ )
	{
		const EntityPtr& entity = entities[i];
		entity->update( delta );
	}
}

//-----------------------------------//

void Group::fixUp()
{
	for( size_t i = 0; i < entities.size(); i++ )
	{
		const EntityPtr& entity = entities[i];
		entity->setParent(this);
	}
}

//-----------------------------------//

} // end namespace