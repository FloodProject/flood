/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

namespace vapor {

//-----------------------------------//

class InputDevice;
struct InputEvent;

class Keyboard;
class Mouse;
class Joystick;

//-----------------------------------//

/**
 * Manages a set of input devices.
 */

class VAPOR_API InputManager : private boost::noncopyable
{
public:

	virtual ~InputManager();

	// Adds a new device that will be managed by this class.
	void addDevice( InputDevice* device );
	
	// Gets a vector with all the known input devices.
	GETTER(Devices, const std::vector<InputDevice*>&, devices)

	// Gets a keyboard device if it exists.
	Keyboard* getKeyboard() const;
	
	// Gets a mouse device if it exists.
	Mouse* getMouse() const;

	// Feeds an external input event to the input manager.
	void processEvent( const InputEvent& event );

	// Creates the default input devices.
	void createDefaultDevices();

protected:

	// Holds all the input devices.
	std::vector< InputDevice* > devices;
};

//-----------------------------------//

} // end namespace