/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"
#include "vapor/scene/Component.h"
#include "vapor/scene/Transform.h"
#include "vapor/scene/Geometry.h"
#include "vapor/math/AABB.h"

namespace vapor { namespace scene {

//-----------------------------------//

class Node;

TYPEDEF_SHARED_POINTER_FROM_CLASS( Node );

//-----------------------------------//

/**
 * Represents a node, that is, an entity of the world held in the scene. 
 * Entities will need to have components to get different properties.
 * For example you can add a Transform component to give the entity a
 * placement in the world. Or you can add a Physics component that will
 * make it react to the world gravity and make it obey the physics laws.
 */

class VAPOR_API Node : public std::enable_shared_from_this< Node >, 
	private boost::noncopyable
{
public:

	explicit Node();
	explicit Node( const std::string& name );
	
	virtual ~Node();

	// Sets the parent of the node.
	void setParent( NodePtr parent );

	// Gets the parent of the node.
	NodePtr getParent() const;

	// Adds a component to this node.
	bool addComponent( ComponentPtr component );

	// Gets a component from this node.
	ComponentPtr getComponent( const std::string& type );

	// Returns all the registered components in this node.
	const std::map< std::string, ComponentPtr >& getComponents();

	// Gets a component from this node.
	template <typename T>
	std::shared_ptr<T> getComponent( const std::string& type )
	{
		ComponentPtr cmp = getComponent( type );
		return std::static_pointer_cast< T >( cmp );
	}

	// Updates all the components of the node.
	virtual void update( float delta );

	// Serializes the node (and its components) to a stream.
	//virtual const std::string save(int indent = 0);

	// Gets the name of the node.
	virtual const std::string& getName() const;

	// Sets the name of the node.
	virtual void setName( const std::string& name );

	// Gets the bounding volume of the node.
	//AABB& getBoundingVolume() const;

	// Gets the associated transform component (if any).
	TransformPtr getTransform();
	Transform* getTransformPtr();

	// Gets the geometries components in the node.
	const std::vector< GeometryPtr >& getGeometry();

	// Is this node visible?
	bool getVisible() const;

	// Sets the visibility of this node.
	void setVisible( bool visible );

private:

	// The name of the node.
	std::string name;

	// Holds the components of the node.
	typedef std::pair< const std::string, ComponentPtr > componentPair;
	std::map< std::string, ComponentPtr > components;

	// Points to the parent node (if any). 
	std::weak_ptr< Node > parent;
	
	// Caches the geometries nodes (for faster lookup when rendering).
	std::vector< GeometryPtr > geometries;

	// Bounding volume used for culling.
	math::AABB boundingVolume;

	// Visibility
	bool isVisible;
};

//-----------------------------------//

} } // end namespaces