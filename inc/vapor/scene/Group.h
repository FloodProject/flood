/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"
#include "vapor/scene/Node.h"

namespace vapor {
	namespace scene {

//-----------------------------------//

/**
 * Groups are nodes that have the special property of being able to be
 * attached to each other. If you attach a node to another node, they will
 * be in a hierarchical relationship and the parent node will influence the 
 * child node transformations.
 */

class VAPOR_API Group : public Node
{
public:

	Group( const std::string& name );
	virtual ~Group();

	// Adds child to the end of the child list.  Returns the index of
	// the new child node that has been added.
	virtual int add( NodePtr child );

	// Inserts child after child at i'th index.  Returns the index of
	// the new child inserted, if there is no existing
	// child at 'i' then -1 is returned.
	virtual int insert( int i, NodePtr child );

	// Removes child at index i, if i is out of bounds returns false,
	// if i is in bounds and child was removed, returns true
	virtual bool remove( int i );

	// Returns null pointer if cannot find index i
	NodePtr get( uint i ) const;

	// Gets the children of this group.
	const std::vector< NodePtr >& getChildren() { return children; }

	// Returns index for child with the same pointer, if child cannot
	// be found, -1 is returned
	int index( NodePtr child ) const;

	// Returns number of children in this group
	int count( ) const;

	// Updates this group of nodes.
	virtual void update( float delta );

	// Serialization of the nodes.
	virtual const std::string save( int indent = 0 );

protected:

	// Holds a group of nodes.
	std::vector< NodePtr > children;
};

//-----------------------------------//

TYPEDEF_SHARED_POINTER_FROM_CLASS( Group );

//-----------------------------------//

} } // end namespaces