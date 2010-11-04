/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/scene/Node.h"

namespace vapor {

//-----------------------------------//

/**
 * Groups are nodes that have the special property of being able to be
 * attached to each other. If you attach a node to another node, they will
 * be in a hierarchical relationship and the parent node will influence the 
 * child node transformations.
 */

class VAPOR_API Group : public Node
{
	DECLARE_CLASS_()

public:

	Group();
	Group( const std::string& name );

	// Adds child to the end of the child list.
	void add( const NodePtr& node );

	// Removes node from the group.
	bool remove( const NodePtr& node );

	// Finds a node in the group via its name.
	NodePtr findNode( const std::string& name ) const;

	// Gets the nodes of the group.
	GETTER(Nodes, const std::vector<NodePtr>&, nodes)

	// Updates this group of nodes.
	virtual void update( double delta );

	// Event gets called when a node is added.
	fd::delegate<void(const NodePtr&)> onNodeAdded;

	// Event gets called when a node is removed.
	fd::delegate<void(const NodePtr&)> onNodeRemoved;

protected:

	// Holds the nodes of the group.
	std::vector<NodePtr> nodes;
};

TYPEDEF_SHARED_POINTER_FROM_TYPE( Group );

//-----------------------------------//

} // end namespace