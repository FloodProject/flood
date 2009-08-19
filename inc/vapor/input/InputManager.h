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
		

	

	
class InputManager
{
public:
	
	

private:
	Keyboard keyboard;
	Mouse    mouse;
	Joystick joystick;

	virtual void keyPressed(KeyInfo keycode);
	virtual void keyReleased(KeyInfo keycode);
	virtual void mouseWheelMoved(int motion);
	virtual void mouseButtonPressed(MouseButtonEvent button);
	virtual void mouseButtonReleased(MouseButtonEvent button);
	virtual void mouseMoved(MouseMoveEvent move);
	virtual void mouseEntered();
	virtual void mouseLeft();
	virtual void joyButtonPressed(JoyButtonEvent button);
	virtual void joyButtonReleased(JoyButtonEvent button);
	virtual void joyMoved(JoyMoveEvent move);

}
		
	}
}