/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "scene/Component.h"
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

//-----------------------------------//

/**
 * Represents an entity of the world held in the scene.
 * Entities can have components attached, providing behaviour.
 * For example you can add a Transform component to give the entity a
 * placement in the world. Or you can add a Physics component that will
 * make it react to the world gravity and make it obey the physics laws.
 */

class VAPOR_API Entity : public std::enable_shared_from_this<Entity>
{
	DECLARE_CLASS_()
	DECLARE_UNCOPYABLE(Entity)

public:

	explicit Entity();
	explicit Entity( const std::string& name );
	
	// Gets/sets the name of the node.
	ACESSOR(Name, const std::string&, name);

	// Gets if this node is visible.
	bool isVisible() const;

	// Sets if this node is visible.
	ACESSOR(Visible, bool, visible);

	// Gets the parent of the node.
	EntityPtr getParent() const;

	// Sets the parent of the node.
	SETTER(Parent, const EntityPtr&, parent)

	// Gets the tag of the node.
	bool getTag( int index );

	// Sets the tag of the node.
	void setTag( int index, bool state );

	// Adds a component to this node.
	bool addComponent( const ComponentPtr& component );

	// Removes a component from this node.
	bool removeComponent( const ComponentPtr& component );

	// Adds a transform component to this node.
	bool addTransform();

	// Gets the associated transform component (if any).
	TransformPtr getTransform() const;

	// Returns all the registered components in this node.
	GETTER(Components, const ComponentMap&, components)

	// Gets the geometries components in the node.
	GETTER(Geometry, const std::vector<GeometryPtr>&, geometries)

	// Gets a component from this node.
	template <typename T>
	std::shared_ptr<T> getComponent() const
	{
		ComponentMap::const_iterator it = components.find(&T::getType());
		
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
			const Type& componentType = component->getInstanceType();

			if( componentType.inherits<T>() )
				return std::static_pointer_cast<T>(component);
		}

		return std::shared_ptr<T>();
	}

	// Updates all the components of the node.
	virtual void update( double delta );

	// Events gets called when a component is added.
	Event1<const ComponentPtr&> onComponentAdded;

	// Events gets called when a component is removed.
	Event1<const ComponentPtr&> onComponentRemoved;

protected:

	// Name of the node.
	std::string name;

	// Visibility of the node.
	bool visible;

	// Tags of the node.
	std::bitset<32> tags;

	// Components of the node.
	ComponentMap components;

	// Parent node of the node.
	EntityWeakPtr parent;
	
	// Caches geometry components.
	std::vector<GeometryPtr> geometries;
};

//-----------------------------------//

} // end namespace