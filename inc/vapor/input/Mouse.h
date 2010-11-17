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
 * Holds the mouse state.
 */

struct VAPOR_API MouseInfo
{
	MouseInfo();

	// Position state.
	short x;
	short y;
	bool insideWindow;
	
	// Buttons state.
	bool leftButton;
	bool rightButton;
	bool middleButton;
	bool aux1Button;
	bool aux2Button;
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

	// Mouse move event.
	Event1< const MouseMoveEvent& > onMouseMove;

	// Mouse drag event.
	Event1< const MouseDragEvent& > onMouseDrag;
	
	// Mouse button press event.
	Event1< const MouseButtonEvent& > onMouseButtonPress;
	
	// Mouse button release event.
	Event1< const MouseButtonEvent& > onMouseButtonRelease;
	
	// Mouse wheel move event.
	Event1< const MouseWheelEvent& > onMouseWheelMove;
	
	// Mouse enter window event.
	Event0<> onMouseEnter;

	// Mouse exit window event.
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
