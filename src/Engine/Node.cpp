/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#include "vapor/scene/Node.h"

namespace vapor {
	namespace scene {

//-----------------------------------//

Node::Node(Node* parent)
	: parent(parent)
{

}

//-----------------------------------//

Node::~Node()
{

}

//-----------------------------------//

void Node::setParent( Node* parent )
{
	this->parent = parent;
}

//-----------------------------------//

} } // end namespaces