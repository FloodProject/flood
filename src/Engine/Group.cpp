/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/scene/Group.h"

namespace vapor {
	namespace scene {

//-----------------------------------//

int Group::add(shared_ptr<Node> child)
{
	shared_ptr<Node> ptr(this);
	child->setParent(ptr);
	children.push_back(child);
	return children.size()-1;
}

//-----------------------------------//

int Group::insert( int i, shared_ptr<Node> child )
{
	return 1;
}

//-----------------------------------//

bool Group::remove( int i )
{
	return false;
}

//-----------------------------------//

} } // end namespaces