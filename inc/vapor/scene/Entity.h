/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/Object.h"
#include "Event.h"
#include "Component.h"

FWD_DECL_SHARED(Entity)
FWD_DECL_SHARED_WEAK(Entity)

FWD_DECL_SHARED(Transform)
FWD_DECL_SHARED(Geometry)

namespace vapor {

//-----------------------------------//

typedef std::map< const Class*, ComponentPtr > ComponentMap;
typedef std::pair< const Class*, ComponentPtr > ComponentMapPair;

/**
 * An entity represents a game object, a container of game components.
 * The components provide behaviour to the entity. An entity can be
 * tagged, which can be useful to provide some different behaviour in
 * the engine, like culling. Each entity has a Transform component
 * by default to give it a placement in the world scene.
 */

class VAPOR_API Entity : public Object
{
	DECLARE_CLASS_()
	DECLARE_UNCOPYABLE(Entity)

public:

	Entity();
	Entity( const std::string& name );
	
	// Gets the name of the entity.
	GETTER(Name, const std::string&, name);
	
	// Sets the name of the entity.
	void setName(const std::string& name);

	// Gets if the entity is visible.
	bool isVisible() const;

	// Sets if the entity is visible.
	SETTER(Visible, bool, visible)

	// Gets the parent of the entity.
	Entity* getParent() const;

	// Sets the parent of the entity.
	SETTER(Parent, Entity*, parent)

	// Gets the tag of the entity.
	bool getTag( int index );

	// Sets the tag of the entity.
	void setTag( int index, bool state );

	// Adds a component to this entity.
	bool addComponent( const ComponentPtr& component );

	// Removes a component from this entity.
	bool removeComponent( const ComponentPtr& component );

	// Gets a component of the given type name.
	ComponentPtr getComponent(const std::string& type) const;

	// Gets a component of the given type.
	ComponentPtr getComponent(const Type& type) const;

	// Gets a component of the given type.
	ComponentPtr getComponentFromFamily(const Type& type) const;

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

protected:

	// Name of the entity.
	std::string name;

	// Visibility of the entity.
	bool visible;

	// Tags of the entity.
	int tags;

	// Components of the entity.
	std::vector<ComponentPtr> components;

	// Components map of the entity.
	ComponentMap componentsMap;

	// Parent entity.
	Entity* parent;
};

//-----------------------------------//

} // end namespace