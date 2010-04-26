/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

namespace vapor { namespace input {

struct MouseInfo;

//-----------------------------------//

/**
 * Different types of mouse buttons.
 */

namespace MouseButton
{
	enum Enum
	{
		// Main mouse buttons
		Left,
		Right,
		Middle,

		// Extra mouse buttons
		Mouse4,
		Mouse5,
	};
}

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
		// Occurs when the mouse is dragged
		MouseDrag,
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
 * Generic mouse event that will be inherited by the more specific 
 * mouse event types. This will be used for communication between
 * the input manager, so it has private members and then adds friend
 * access to the Mouse class. That way the private data is hidden to
 * the public.
 */

struct MouseEvent : public input::Event
{
	friend class Mouse;
	
	MouseEvent( MouseEventType::Enum eventType );

protected:

	MouseEventType::Enum eventType;
};

//-----------------------------------//

/**
 * Mouse event that occurs when the mouse is moved and provides the
 * current mouse position on the screen in screen-coordinates.
 */

struct MouseMoveEvent : public MouseEvent
{
	MouseMoveEvent();

	short x;
	short y;
};

//-----------------------------------//

/**
 * Mouse event that occurs when the mouse is dragged. Use MouseInfo
 * to get more information about which mouse buttons are pressed.
 */

struct MouseDragEvent : public MouseEvent
{
	MouseDragEvent();

	short dx;
	short dy;

	short x;
	short y;
};

//-----------------------------------//
 
/**
 * Mouse event that occurs when a button of the mouse is pressed.
 * It holds the button that was pressed and also the position of the 
 * mouse in the window at the time of the click.
 */

struct MouseButtonEvent : public MouseEvent
{
	MouseButtonEvent( MouseEventType::Enum eventType );

	short x;
	short y;

	MouseButton::Enum button;
};

//-----------------------------------//

/**
 * Mouse events that occurs when the mouse wheel is scrolled.
 * In this case the extra information is a delta value that specifies
 * the relative amount of the mouse wheel that was scrolled.
 */

struct MouseWheelEvent : public MouseEvent
{
	MouseWheelEvent();

	short delta;
};

//-----------------------------------//

} } // end namespaces
