/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"
#include "vapor/math/Matrix.h"

using namespace vapor::math;

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