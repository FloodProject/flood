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
		namespace Mouse
			{
			enum Enum
				{
				Left,
				Right,
				Middle,
				XButton1,
				XButton2,

				};
			}
struct MouseInfo
{
struct MouseInfo():x(0), y(0), leftButton(false),
	rightButton(false), middleButton(false), XButton1(false),
	XButton2(false){}
int x;
int y;
bool leftButton;
bool rightButton;
bool middleButton;
bool XButton1;
bool XButton2;
bool inWindow;
}
struct MouseMoveEvent
{
  int x;
  int y;
};
 
struct MouseButtonEvent
{
  Mouse::Button button;
  int  x;
  int  y;
};



class Mouse : public Device
{
friend class InputManager;
public:
	Mouse();
	MouseInfo getMouseInfo();
	  
private:
	MouseInfo mouseinfo;
	void mouseButtonPressed(MouseButtonEvent press);
	void mouseButtonReleased(MouseButtonEvent release);
	void mouseButtonMoved(MouseButtonEvent move);
}


}}