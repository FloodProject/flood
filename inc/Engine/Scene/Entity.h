/************************************************************************
*
* Flood Project � (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Engine/API.h"
#include "Core/Object.h"
#include "Core/Event.h"
#include "Engine/Scene/Component.h"

FWD_DECL_INTRUSIVE(Entity)
FWD_DECL_INTRUSIVE(Component)
FWD_DECL_INTRUSIVE(Transform)
FWD_DECL_INTRUSIVE(Geometry)

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

API_SCENE Entity* EntityCreate( Allocator* );

typedef HashMap<Class*, ComponentPtr> ComponentMap;
typedef std::pair<Class*, ComponentPtr> ComponentMapPair;

/**
 * An entity represents a game object, a container of game components.
 * The components provide behaviour to the entity. An entity can be
 * tagged, which can be useful to provide some different behaviour in
 * the engine, like culling. Each entity has a Transform component
 * by default to give it a placement in the world scene.
 */

API_SCENE REFLECT_DECLARE_CLASS(Entity)

class API_SCENE Entity : public Object
{
	REFLECT_DECLARE_OBJECT(Entity);
	DECLARE_UNCOPYABLE(Entity)
	DECLARE_FRIENDS(Entity)

	Entity();
	Entity( const String& name );
	~Entity();

public:
	
	// Gets the name of the entity.
	GETTER(Name, const String&, name);
	
	// Sets the name of the entity.
	void setName(const String& name);

	// Gets if the entity is visible.
	bool isVisible() const;

	// Sets if the entity is visible.
	SETTER(Visible, bool, visible)

	// Gets the parent of the entity.
	Entity* getParent() const;

	// Sets the parent of the entity.
	SETTER(Parent, Entity*, parent)

	// Gets the tag of the entity.
	bool getTag( int32 index ) const;

	// Sets the tag of the entity.
	void setTag( int32 index, bool state );

	// Adds a component to this entity.
	bool addComponent( const ComponentPtr& component );

	// Removes a component from this entity.
	bool removeComponent( const ComponentPtr& component );

	// Gets a component of the given type name.
	ComponentPtr getComponent(const char* name) const;

	// Gets a component of the given type.
	ComponentPtr getComponent(Class* klass) const;

	// Gets a component of the given type.
	ComponentPtr getComponentFromFamily(Class* klass) const;

	// Gets a component from this entity.
	FLD_IGNORE template <typename T>
	RefPtr<T> getComponent() const
	{
		return RefCast<T>( getComponent(T::getStaticType()) );
	}

	// Gets the first found component inheriting the given type.
	FLD_IGNORE template<typename T>
	RefPtr<T> getComponentFromFamily()
	{
		return RefCast<T>( getComponentFromFamily(T::getStaticType()) );
	}

	// Adds a transform component to this entity.
	bool addTransform();

	// Gets the associated transform component (if any).
	TransformPtr getTransform() const;

	// Returns all the registered components in this entity.
	GETTER(Components, const ComponentMap&, componentsMap)

	// Gets the geometries components in the entity.
	Vector<GeometryPtr> getGeometry() const;

	// Updates all the components of the entity.
	virtual void update( float delta );

	// Fix-up serialization.
	virtual void fixUp() OVERRIDE;

	// Sends event notifications.
	void sendEvents();

	// Events gets called when a component is added.
	Event1<const ComponentPtr&> onComponentAdded;

	// Events gets called when a component is removed.
	Event1<const ComponentPtr&> onComponentRemoved;

protected:

	// Name of the entity.
	String name;

	// Visibility of the entity.
	bool visible;

	// Tags of the entity.
	uint32 tags;

	// Components of the entity.
	Vector<ComponentPtr> components;

	// Components map of the entity.
	ComponentMap componentsMap;

	// Parent entity.
	Entity* parent;
};

//-----------------------------------//

NAMESPACE_ENGINE_END