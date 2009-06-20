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

class Group : public Node
{
public:

  // adds child to the end of the child list
  void add( shared_ptr<Node> child );
  
  // inserts child after child at i'th index
  void insert( int i, shared_ptr<Node> child );
  
  // removes all children matching the child pointer, if there is more 
  // than one matching it will be removed
  void remove( shared_ptr<Node> child );
  
  // removes child at index i, if i is out of bounds returns false,
  // if i is in bounds and child was removed, returns true
  bool remove( int i );
  
  // returns null pointer if cannot find index i
  shared_ptr<Node> get( int i ) const;
  
  // returns -1 if cannot find child
  int indexOf( shared_ptr<Node> child ) const;
  
private:
  std::vector<shared_ptr<Node>> children;
};

} } // end namespaces