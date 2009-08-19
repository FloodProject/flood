/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"

#include "vapor/input/Device.h"

namespace vapor {
	namespace input {
		
/**
 * Manages a set of input devices.
 */

class InputManager
{
public:
	
	// Gets a vector with all the known input devices.
	const std::vector< Device >& getDevices();

	void feedEvent( const Event& event );

private:

	std::vector< Device > devices;
}

} } // end namespaces