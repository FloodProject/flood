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
#include "vapor/input/Event.h"

namespace vapor {
	namespace input {
		namespace MouseButton
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
			
		namespace MouseEventType
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
};

struct MouseEvent : public input::Event
{
	friend class Mouse;
	MouseEvent(MouseEventType::Enum eventType): 
	Event( DeviceType::Mouse ), eventType(eventType){}

	
	private:
		MouseEventType::Enum eventType;
};

struct MouseMoveEvent : public MouseEvent
{

	friend class Mouse;
	MouseMoveEvent(int x, int y): MouseEvent(MouseEventType::MouseMove),
			x(x), y(y) {}
	int x;
	int y;
};
 
struct MouseButtonEvent : public MouseEvent
{
	friend class Mouse;
	MouseButtonEvent(int x, int y, MouseButton::Enum button, MouseEventType::Enum eventType)
	: MouseEvent(eventType), x(x), y(y), button(button) {}
	MouseButton::Enum button;
	int  x;
	int  y;
};

struct MouseWheelEvent : public MouseEvent
{
	friend class Mouse;
	MouseWheelEvent(int delta, int y): MouseEvent(MouseEventType::MouseWheelMove),
			delta(delta){}
	int  delta;
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

	void mouseButtonPressed(const MouseEvent& mevt);
	void mouseButtonReleased(const MouseEvent& mevt);
	void mouseMoved(const MouseEvent& mevt);
	void mouseEnter();
	void mouseExit();
	void mouseWheelMove(const MouseEvent& mevt);
	MouseInfo mouseinfo;
	
};


}}