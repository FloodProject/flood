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

//-----------------------------------//

namespace MouseButton
{
	enum Enum
	{
		// Main mouse buttons
		Left,
		Right,
		Middle,

		// Extra mouse buttons
		XButton1,
		XButton2,
	};
}

//-----------------------------------//

/**
 * Holds all the mouse state.
 */

struct MouseInfo
{
	MouseInfo();

	int x;
	int y;
	
	bool leftButton;
	bool rightButton;
	bool middleButton;
	
	bool XButton1;
	bool XButton2;
	
	bool inWindow;
};

//-----------------------------------//

/**
 * Different types of mouse events.
 */

namespace MouseEventType
{
	enum Enum
	{
		// Occurs when the mouse wheel is scrolled
		MouseWheelMove,
		// Occurs when the mouse is moved
		MouseMove,
		// Occurs when a mouse button is pressed
		MousePress,
		// Occurs when a mouse button is released
		MouseRelease,
		// Occurs when the mouse cursor enters the window
		MouseEnter,
		// Occurs when the mouse cursor exits the window
		MouseExit,
	};
}

//-----------------------------------//

/**
 * Generic mouse event that will be inherited by the more specific mouse 
 * event types. This will be used for public and private communication
 * between the input manager, so it has private members and then adds
 * friend acess to the event handler. That way the private data is hidden
 * to the public.
 */

struct MouseEvent : public input::Event
{
	friend class Mouse;
	
	MouseEvent( MouseEventType::Enum eventType );

private:

	MouseEventType::Enum eventType;
};

//-----------------------------------//

/**
 * Mouse event that occurs when the mouse moves. It holds the new position
 * of the mouse cursor in the screen.
 */

struct MouseMoveEvent : public MouseEvent
{
	friend class Mouse;
	
	MouseMoveEvent( int x, int y );

	int x;
	int y;
};

//-----------------------------------//
 
/**
 * Mouse event that occurs when a button of the mouse is pressed.
 * It holds the button that was pressed and also the position of the 
 * mouse in the window at the time of the click.
 */

struct MouseButtonEvent : public MouseEvent
{
	friend class Mouse;

	MouseButtonEvent( int x, int y, 
		MouseButton::Enum button, MouseEventType::Enum eventType );
	
	MouseButton::Enum button;
	
	int  x;
	int  y;
};

//-----------------------------------//

/**
 * Mouse events that occurs when the mouse wheel is scrolled.
 * In this case the extra information is a delta value that specifies
 * the ammount of the mouse wheel that was scrolled.
 */

struct MouseWheelEvent : public MouseEvent
{
	friend class Mouse;
	
	MouseWheelEvent( int delta, int y );

	int  delta;
};

//-----------------------------------//

class Mouse : public Device
{
	//friend class InputManager;

public:

	Mouse();
	virtual ~Mouse();
	
	// Gets a structure with the current mouse state.
	const MouseInfo& getMouseInfo();
	  
	// Processes an event (only if it's a mouse event).
	virtual void processEvent( const input::Event& event );

	// Return this device as a mouse.
	virtual const input::DeviceType::Enum getType();

private:

	// Occurs when a mouse button is pressed.
	void mouseButtonPressed(const MouseButtonEvent& mevt);
	
	// Occurs when a mouse button is released.
	void mouseButtonReleased(const MouseButtonEvent& mevt);
	
	// Occurs when the mouse is moved.
	void mouseMoved(const MouseMoveEvent& mevt);
	
	// Occurs when the mouse cursor enters the window.
	void mouseEnter();
	
	// Occurs when the mouse cursor exits the window.
	void mouseExit();
	
	// Occurs when the mouse wheel is scrolled.
	void mouseWheelMove(const MouseWheelEvent& mevt);
	
	// Holds the current state of the mouse.
	MouseInfo mouseInfo;
};

//-----------------------------------//

} } // end namespaces