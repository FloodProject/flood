/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Object.h"
#include "Event.h"

FWD_DECL_SHARED(Component)

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

class VAPOR_API Entity : public Object, public std::enable_shared_from_this<Entity>
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
	EntityPtr getParent() const;

	// Sets the parent of the entity.
	SETTER(Parent, const EntityPtr&, parent)

	// Gets the tag of the entity.
	bool getTag( int index );

	// Sets the tag of the entity.
	void setTag( int index, bool state );

	// Adds a component to this entity.
	bool addComponent( const ComponentPtr& component );

	// Removes a component from this entity.
	bool removeComponent( const ComponentPtr& component );

	// Adds a transform component to this entity.
	bool addTransform();

	// Gets the associated transform component (if any).
	TransformPtr getTransform() const;

	// Returns all the registered components in this entity.
	GETTER(Components, const ComponentMap&, components)

	// Gets the geometries components in the entity.
	GETTER(Geometry, const std::vector<GeometryPtr>&, geometries)

	// Gets a component from this entity.
	template <typename T>
	std::shared_ptr<T> getComponent() const
	{
		ComponentMap::const_iterator it = components.find(&T::getStaticType());
		
		if( it == components.end() )
			return std::shared_ptr<T>();

		return std::static_pointer_cast<T>((*it).second);
	}

	// Gets the first found component inheriting the given type.
	template<typename T>
	std::shared_ptr<T> getTypedComponent()
	{
		ComponentMap::const_iterator it;
		for( it = components.cbegin(); it != components.cend(); it++ )
		{
			const ComponentPtr& component = it->second;
			const Type& componentType = component->getType();

			if( componentType.inherits<T>() )
				return std::static_pointer_cast<T>(component);
		}

		return std::shared_ptr<T>();
	}

	// Updates all the components of the entity.
	virtual void update( double delta );

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
	ComponentMap components;

	// Parent entity.
	EntityWeakPtr parent;
	
	// Caches geometry components.
	std::vector<GeometryPtr> geometries;
};

//-----------------------------------//

} // end namespace