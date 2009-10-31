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

namespace vapor {
	namespace scene {

//-----------------------------------//

class Node;

TYPEDEF_SHARED_POINTER_FROM_CLASS( Node );

//-----------------------------------//

/**
 * Represents a node, that is, an entity in the scene. 
 * The entities are divided into two main types:
 *   ~ Transform (entities that can be placed in the scene)
 *   ~ Group (group of other entities in the scene)
 */

class VAPOR_API Node : public std::enable_shared_from_this< Node >, 
	private boost::noncopyable
{
public:

	explicit Node();
	virtual ~Node();

	// Sets the parent of the node.
	void setParent( NodePtr parent );

	// Gets the parent of the node.
	NodePtr getParent() const;

	// Adds a component to this node.
	bool addComponent( ComponentPtr component );

	// Gets a component from this node.
	ComponentPtr getComponent( const std::string& type );

	// Gets a component from this node.
	template <typename T>
	std::shared_ptr<T> getComponent( const std::string& type )
	{
		ComponentPtr cmp = getComponent( type );
		return std::static_pointer_cast< T >( cmp );
	}

	// Gets the bounding volume of the node.
	//AABB getBoundingVolume() const;

	virtual void update( float delta );

	//virtual const std::string save(int indent = 0);

	virtual const std::string name() const;

	TransformPtr getTransform();

	const std::vector< GeometryPtr >& getGeometry();
  
private:

	std::map< std::string, ComponentPtr > components;
	typedef std::pair< const std::string, ComponentPtr > componentPair;

	std::weak_ptr< Node > parent;
	std::vector< GeometryPtr > geometries;
	//AABB boundingVolume;
};

//-----------------------------------//

} } // end namespaces