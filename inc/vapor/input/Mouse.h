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
			
		namespace MouseEvent
			{
			enum Enum
				{
				MouseWheelMove,
				MouseMove,
				MousePress,
				MouseRelease,
				MouseEnter,
				MouseExit,

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

struct MouseEvent : public input::Event
{
	private:
		MouseEvent::Enum eventType;
}

struct MouseMoveEvent : public MouseEvent
{
	
	int x;
	int y;
};
 
struct MouseButtonEvent : public MouseEvent
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
	virtual ~Mouse();
	MouseInfo getMouseInfo();
	  
private:
	// Processes an event (only if it's a mouse event).
	virtual void processEvent( const input::Event& event );

	// Return this device as a mouse.
	virtual const input::DeviceType::Enum getType();

	void mouseButtonPressed(MouseButtonEvent press);
	void mouseButtonReleased(MouseButtonEvent release);
	void mouseButtonMoved(MouseButtonEvent move);
	MouseInfo mouseinfo;
	
}


}}