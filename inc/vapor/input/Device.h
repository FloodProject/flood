/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"

#include "vapor/input/Event.h"

namespace vapor {
	namespace input {

//-----------------------------------//

/**
 * Input device.
 */

class Device
{
public:

	Device();
	virtual ~Device();

	// Processes an event.
	virtual void processEvent( const input::Event& event ) = 0;

	// Gets the type of this device.
	virtual const input::DeviceType::Enum getType() = 0;
};

//-----------------------------------//

} } // end namespaces