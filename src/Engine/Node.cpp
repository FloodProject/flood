/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/scene/Node.h"

namespace vapor {
	namespace scene {

//-----------------------------------//

Node::Node(shared_ptr<Node> parent)
	: parent(parent)
{

}

//-----------------------------------//

Node::~Node()
{

}

//-----------------------------------//

void Node::setParent( shared_ptr<Node> parent )
{
	this->parent = parent;
}

//-----------------------------------//

} } // end namespaces