/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/input/Device.h"
#include "vapor/input/MouseEvents.h"
#include "Event.h"

namespace vapor {

//-----------------------------------//

/**
 * Holds all the mouse state.
 */

struct VAPOR_API MouseInfo
{
	MouseInfo();

	short x;
	short y;
	
	bool leftButton;
	bool rightButton;
	bool middleButton;
	
	bool Mouse4;
	bool Mouse5;
	
	bool insideWindow;
};

//-----------------------------------//

class VAPOR_API Mouse : public InputDevice
{
public:
	  
	// Returns whether a given mouse button is pressed.
	bool isButtonPressed( MouseButton::Enum button ) const;

	// Processes an event (only if it's a mouse event).
	virtual void processEvent( const InputEvent& event );

	// Gets a structure with the current mouse state.
	GETTER(MouseInfo, const MouseInfo&, mouseInfo)

	// Return this device as a mouse.
	GETTER(Type, const InputDeviceType::Enum, InputDeviceType::Mouse)

	// Events

	Event1< const MouseMoveEvent& > onMouseMove;
	Event1< const MouseDragEvent& > onMouseDrag;
	
	Event1< const MouseButtonEvent& > onMouseButtonPress;
	Event1< const MouseButtonEvent& > onMouseButtonRelease;
	
	Event1< const MouseWheelEvent& > onMouseWheelMove;
	
	Event0<> onMouseEnter;
	Event0<> onMouseExit;

private:

	// Occurs when a mouse button is pressed.
	void mouseButtonPressed(const MouseButtonEvent& mevt);
	
	// Occurs when a mouse button is released.
	void mouseButtonReleased(const MouseButtonEvent& mevt);
	
	// Occurs when the mouse is moved.
	void mouseMoved(const MouseMoveEvent& mevt);

	// Occurs when the mouse is dragged.
	void mouseDragged(const MouseDragEvent& mevt);
	
	// Occurs when the mouse cursor enters the window.
	void mouseEnter();
	
	// Occurs when the mouse cursor exits the window.
	void mouseExit();
	
	// Occurs when the mouse wheel is scrolled.
	void mouseWheelMove(const MouseWheelEvent& mevt);

	// Sets the button state in the mouse info.
	void setMouseButton( MouseButton::Enum btn, bool state );
	
	// Holds the current state of the mouse.
	MouseInfo mouseInfo;
};

//-----------------------------------//

} // end namespace
