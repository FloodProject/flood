/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/input/Type.h"

namespace vapor {
	namespace input {

//-----------------------------------//

/**
 * Base struct for input events.
 */

struct Event
{
	Event( DeviceType::Enum );

	DeviceType::Enum deviceType;
};

//-----------------------------------//

} } // end namespaces