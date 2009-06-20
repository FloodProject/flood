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

void Group::add(shared_ptr<Node> child)
{
	children.push_back(child);
}

//-----------------------------------//

} } // end namespaces