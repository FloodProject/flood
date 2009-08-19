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
	namespace input {	
		namespace joystick
			{
			enum Enum
				{
				AxisX,
				AxisY,
				AxisZ,
				AxisR,
				AxisU,
				AxisV,
				AxisPOV,
				};
			}
		struct JoyMoveEvent
			{
			unsigned int JoystickId;
			Enum		 Axis;
			float        Position;
			};

		struct JoyButtonEvent
			{
			unsigned int JoystickId;
			unsigned int Button;
			};

		class Joystick : public Device
			{
			friend class InputManager;
			
			}
	}
}