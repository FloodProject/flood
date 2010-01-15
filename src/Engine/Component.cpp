/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/scene/Component.h"

namespace vapor { namespace scene {

//-----------------------------------//

Component::Component()
{

}

//-----------------------------------//

Component::~Component()
{

}

//-----------------------------------//

void Component::setNode( NodePtr node )
{
	this->node = node ;
}

//-----------------------------------//

NodePtr Component::getNode() const
{
	return node.lock();
}

//-----------------------------------//

} } // end namespaces