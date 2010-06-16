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

typedef std::map< std::string, ComponentPtr > ComponentMap;
typedef std::pair< const std::string, ComponentPtr > ComponentMapPair;


//-----------------------------------//

/**
 * Represents a node, that is, an entity of the world held in the scene. 
 * Entities will need to have components to get different properties.
 * For example you can add a Transform component to give the entity a
 * placement in the world. Or you can add a Physics component that will
 * make it react to the world gravity and make it obey the physics laws.
 */

class VAPOR_API Node : public std::enable_shared_from_this< Node >, 
	private boost::noncopyable, public Serializable
{
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
	ComponentPtr getComponent( const std::string& type ) const;

	// Returns all the registered components in this node.
	GETTER(Components, const ComponentMap&, components)

	// Gets a component from this node.
	template <typename T>
	std::shared_ptr<T> getComponent( const std::string& type ) const
	{
		const ComponentPtr& cmp = getComponent( type );
		return std::static_pointer_cast< T >( cmp );
	}

	// Removes a component from this node.
	bool removeComponent( const std::string& name );

	// Updates all the components of the node.
	virtual void update( double delta );

	// Gets/sets the name of the node.
	ACESSOR(Name, const std::string&, name);

	// Gets the associated transform component (if any).
	TransformPtr getTransform() const;

	// Gets the geometries components in the node.
	GETTER(Geometry, const std::vector<GeometryPtr>&, geometries)

	// Is this node visible?
	bool isVisible() const;

	// Sets the visibility of this node.
	ACESSOR(Visible, bool, _isVisible);

	// Gets/sets the node's tag state.
	bool getTag( int index );
	void setTag( int index, bool state );

	DECLARE_SERIALIZABLE();

private:

	// The name of the node.
	std::string name;

	// Holds the components of the node.
	ComponentMap components;

	// Points to the parent node (if any). 
	NodeWeakPtr parent;
	
	// Caches the geometries nodes (for faster lookup when rendering).
	std::vector<GeometryPtr> geometries;

	// Visibility
	bool _isVisible;

	// Bitset used to store useful information about the node.
	std::bitset<32> tag;
};

//-----------------------------------//

} // end namespace