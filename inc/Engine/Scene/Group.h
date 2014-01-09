/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Engine/Scene/Entity.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

/**
 * Groups are nodes that have the special property of being able to be
 * attached to each other. If you attach a node to another node, they will
 * be in a hierarchical relationship and the parent node will influence the 
 * child node transformations.
 */

API_SCENE REFLECT_DECLARE_CLASS(Group)

class API_SCENE Group : public Entity
{
	REFLECT_DECLARE_OBJECT(Group)

public:

	Group();
	Group( const String& name );
	~Group();

	// Adds an entity to the group.
	void add( const EntityPtr& entity );

	// Removes an entity from the group.
	bool remove( const EntityPtr& entity );

	// Finds an entity in the group via its name.
	EntityPtr findEntity( const String& name ) const;

	// Gets the entities of the group.
	GETTER(Entities, const Vector<EntityPtr>&, entities)

	// Updates the group.
	virtual void update( float delta ) OVERRIDE;

	// Fix-up serialization.
	virtual void fixUp() OVERRIDE;

	// Event gets called when an entity is added/removed.
	Event1<const EntityPtr&> onEntityAdded;
	Event1<const EntityPtr&> onEntityRemoved;

	// Event gets called when an entity component is added/removed.
	Event1<const ComponentPtr&> onEntityComponentAdded;
	Event1<const ComponentPtr&> onEntityComponentRemoved;

	// Event gets called when its contents change.
	Event0<> onEntityChanged;

protected:

	// Holds the entities of the group.
	Vector<EntityPtr> entities;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Group );

//-----------------------------------//

NAMESPACE_ENGINE_END