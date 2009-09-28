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

//-----------------------------------//

class Node;

typedef tr1::shared_ptr< Node > NodePtr;

//-----------------------------------//

/**
 * Represents a node, that is, an entity in the scene. 
 * The entities are divided into two main types:
 *   ~ Transformable (entities that can be placed in the scene)
 *   ~ Group (group of other entities in the scene)
 */

class VAPOR_API Node : public tr1::enable_shared_from_this< Node >, 
	private boost::noncopyable
{
public:

  explicit Node( NodePtr parent = NodePtr() );
  virtual ~Node();
  
  // Sets the parent of the node.
  void setParent( NodePtr parent );
  
  // Gets the parent of the node.
  NodePtr getParent() const;
  
  // Gets the bounding volume of the node.
  //AABB getBoundingVolume() const;

  virtual void update( double delta ) = 0;

  virtual const std::string save(int indent = 0) = 0;

  virtual const std::string name() const;
  
private:

	tr1::weak_ptr< Node > parent;
	//AABB boundingVolume;
};

//-----------------------------------//

} } // end namespaces