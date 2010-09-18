/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/scene/Component.h"
#include "vapor/scene/Transform.h"
#include "vapor/scene/Geometry.h"

namespace vapor {

//-----------------------------------//

class Node;

TYPEDEF_SHARED_POINTER_FROM_TYPE( Node );
TYPEDEF_SHARED_WEAK_POINTER_FROM_TYPE( Node );

typedef std::map< const Class*, ComponentPtr > ComponentMap;
typedef std::pair< const Class*, ComponentPtr > ComponentMapPair;

//-----------------------------------//

/**
 * Represents a node, that is, an entity of the world held in the scene. 
 * Nodes can have components that attach behaviour and state to the node.
 * For example you can add a Transform component to give the entity a
 * placement in the world. Or you can add a Physics component that will
 * make it react to the world gravity and make it obey the physics laws.
 */

class VAPOR_API Node : private boost::noncopyable, public std::enable_shared_from_this<Node>
	
{
	DECLARE_CLASS_()

public:

	explicit Node();
	explicit Node( const std::string& name );
	
	// Gets the parent of the node.
	NodePtr getParent() const;

	// Sets the parent of the node.
	SETTER(Parent, const NodePtr&, parent)

	// Adds a component to this node.
	bool addComponent( const ComponentPtr& component );

	// Adds a transform component to this node.
	bool addTransform();

	// Gets a component from this node.
	template <typename T>
	std::shared_ptr<T> getComponent() const
	{
		const Class* type = &T::getType();

		ComponentMap::const_iterator it = components.find(type);
		
		if( it == components.end() )
			return std::shared_ptr<T>();

		return std::static_pointer_cast<T>((*it).second);
	}

	// Gets the first found component inheriting the given type.
	template<typename T>
	std::shared_ptr<T> getTypedComponent()
	{
		foreach( const ComponentMapPair& p, components )
		{
			const ComponentPtr& component = p.second;
			const Type& componentType = component->getInstanceType();

			if( componentType.inherits<T>() )
				return std::static_pointer_cast<T>(component);
		}

		return std::shared_ptr<T>();
	}

	// Returns all the registered components in this node.
	GETTER(Components, const ComponentMap&, components)

	// Gets a component from this node.
	template <typename T>
	std::shared_ptr<T> getComponent( const std::string& type ) const
	{
		const ComponentPtr& comp = getComponent<T>();
		return std::static_pointer_cast<T>(comp);
	}

	// Removes a component from this node.
	bool removeComponent( const ComponentPtr& component );

	// Updates all the components of the node.
	virtual void update( double delta );

	// Gets/sets the name of the node.
	ACESSOR(Name, const std::string&, name);

	// Gets the associated transform component (if any).
	TransformPtr getTransform() const;

	// Gets the geometries components in the node.
	GETTER(Geometry, const std::vector<GeometryPtr>&, geometries)

	// Gets if this node is visible.
	bool isVisible() const;

	// Sets if this node is visible.
	ACESSOR(Visible, bool, visible);

	// Gets/sets the node tag state.
	bool getTag( int index );
	void setTag( int index, bool state );

private:

	// Name of the node.
	std::string name;

	// Visibility of the node.
	bool visible;

	// Tags of the node..
	std::bitset<32> tags;

	// Components of the node.
	ComponentMap components;

	// Parent node of the node.
	NodeWeakPtr parent;
	
	// Caches geometry components.
	std::vector<GeometryPtr> geometries;
};

//-----------------------------------//

} // end namespace