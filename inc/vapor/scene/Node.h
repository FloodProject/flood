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

  explicit Node( Node* parent = nullptr );
  virtual ~Node();
  
  // Sets the parent of the node.
  void setParent( Node* parent );
  
  // Gets the parent of the node.
  Node* getParent() const { return parent; }
  
  // Gets the bounding volume of the node.
  //AABB getBoundingVolume() const;

  virtual std::string save(int indent = 0) = 0;

  virtual std::string name() { return "Node"; }
  
private:
	Node* parent;
  //AABB boundingVolume;
};

} } // end namespaces