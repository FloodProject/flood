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

class Node
{
public:

	Node();
	virtual ~Node();

protected:

	Node* parentNode;
	std::vector<Node*> childrenNode;
};

} } // end namespaces