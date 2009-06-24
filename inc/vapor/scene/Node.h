/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"

namespace vapor {
	namespace scene {


/**
 * Represents a node, that is, an entity in the scene. 
 * The entities are divided into two main types:
 *   ~ Transformable (entities that can be placed in the scene)
 *   ~ Group (group of other entities in the scene)
 */

class Node
{
public:

  explicit Node( shared_ptr<Node> parent = shared_ptr<Node>() );
  virtual ~Node();
  
  // Sets the parent of the node.
  void setParent( shared_ptr<Node> parent );
  
  // Gets the parent of the node.
  shared_ptr<Node> getParent() const { return parent; }
  
  // Gets the bounding volume of the node.
  //AABB getBoundingVolume() const;
  
private:
  shared_ptr<Node> parent;
  //AABB boundingVolume;
};



} } // end namespaces