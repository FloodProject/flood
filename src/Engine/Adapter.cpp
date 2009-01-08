/************************************************************************
*
* vaporEngine by triton (2008)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/render/Adapter.h"

namespace vapor {
	namespace render {

//-----------------------------------//

Adapter::Adapter()
{

}

//-----------------------------------//

Adapter::~Adapter()
{

}

//-----------------------------------//

const char* Adapter::getDriver()
{
	return _driverVersion;
}

//-----------------------------------//

const char* Adapter::getVendor()
{
	return _cardVendor;
}

//-----------------------------------//

const char* Adapter::getName()
{
	return _cardName;
}

} } // end namespaces
