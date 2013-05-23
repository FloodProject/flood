/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Engine/API.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

void API_INPUT InputInitialize();
void API_INPUT InputDeinitialize();

class InputDevice;
struct InputEvent;

class Keyboard;
class Mouse;
class Joystick;

class Window;

/**
 * Manages a set of input devices.
 */

class InputManager;
API_INPUT InputManager* GetInputManager();

class API_INPUT InputManager
{
	DECLARE_UNCOPYABLE(InputManager)

public:

	InputManager();
	virtual ~InputManager();

	// Adds a new device that will be managed by this class.
	void addDevice( InputDevice* device );
	
	// Gets a vector with all the known input devices.
	GETTER(Devices, const Array<InputDevice*>&, devices)

	// Gets a keyboard device if it exists.
	Keyboard* getKeyboard() const;
	
	// Gets a mouse device if it exists.
	Mouse* getMouse() const;

	// Feeds an external input event to the input manager.
	void processEvent( const InputEvent& event );

	// Creates the default input devices.
	void createDefaultDevices();

	// Gets the current input window.
	GETTER(Window, Window*, window)

	// Sets the current input window.
	void setWindow(Window* window);

protected:
	
	// Holds all the input devices.
	Array<InputDevice*> devices;

	// Input window.
	Window* window;
};

//-----------------------------------//

NAMESPACE_ENGINE_END