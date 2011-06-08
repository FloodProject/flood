/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Scene/Component.h"
#include "Core/Object.h"
#include "Core/Event.h"

FWD_DECL_SHARED(Entity)
FWD_DECL_SHARED_WEAK(Entity)

FWD_DECL_SHARED(Transform)
FWD_DECL_SHARED(Geometry)

NAMESPACE_BEGIN

//-----------------------------------//

EXTERN_BEGIN

class Entity;
API_ENGINE Entity* EntityCreate( Allocator* );

EXTERN_END

//-----------------------------------//

typedef std::map<Class*, ComponentPtr> ComponentMap;
typedef std::pair<Class*, ComponentPtr> ComponentMapPair;

/**
 * An entity represents a game object, a container of game components.
 * The components provide behaviour to the entity. An entity can be
 * tagged, which can be useful to provide some different behaviour in
 * the engine, like culling. Each entity has a Transform component
 * by default to give it a placement in the world scene.
 */

REFLECT_DECLARE_CLASS(Entity)

class API_ENGINE Entity : public Object
{
	REFLECT_DECLARE_OBJECT(Entity);
	DECLARE_UNCOPYABLE(Entity)
	DECLARE_FRIENDS(Entity)

	Entity();
	Entity( const String& name );

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
	bool getTag( int32 index );

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
	template <typename T>
	std::shared_ptr<T> getComponent() const
	{
		return std::static_pointer_cast<T>( getComponent(T::getStaticType()) );
	}

	// Gets the first found component inheriting the given type.
	template<typename T>
	std::shared_ptr<T> getComponentFromFamily()
	{
		return std::static_pointer_cast<T>( getComponentFromFamily(T::getStaticType()) );
	}

	// Adds a transform component to this entity.
	bool addTransform();

	// Gets the associated transform component (if any).
	TransformPtr getTransform() const;

	// Returns all the registered components in this entity.
	GETTER(Components, const ComponentMap&, componentsMap)

	// Gets the geometries components in the entity.
	std::vector<GeometryPtr> getGeometry() const;

	// Gets the shared pointer from this entity.
	EntityPtr getShared();

	// Updates all the components of the entity.
	virtual void update( float delta );

	// Fix-up serialization.
	virtual void fixUp();

	// Sends event notifications.
	void sendEvents();

	// Events gets called when a component is added.
	Event1<const ComponentPtr&> onComponentAdded;

	// Events gets called when a component is removed.
	Event1<const ComponentPtr&> onComponentRemoved;

public:

	// Name of the entity.
	String name;

	// Visibility of the entity.
	bool visible;

	// Tags of the entity.
	uint32 tags;

	// Components of the entity.
	std::vector<ComponentPtr> components;

	// Components map of the entity.
	ComponentMap componentsMap;

	// Parent entity.
	Entity* parent;
};

//-----------------------------------//

NAMESPACE_END