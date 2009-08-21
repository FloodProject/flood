/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/input/Joystick.h"

namespace vapor {
	namespace input {

//-----------------------------------//

JoystickEvent::JoystickEvent( JoystickEventType::Enum eventType )
	: Event( DeviceType::Joystick )
{

}

//-----------------------------------//

} } // end namespaces