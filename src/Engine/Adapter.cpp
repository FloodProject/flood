/************************************************************************
*
* vaporEngine by triton (2008)
*
*	<http://www.portugal-a-programar.org>
*
* DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
* Version 2, December 2004
* 
* DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
* TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
* 
* 0. You just DO WHAT THE FUCK YOU WANT TO.
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
