/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/CompileOptions.h"

#ifdef VAPOR_INPUT_SFML

#include <SFML/Window.hpp>

#include "vapor/input/InputManager.h"

#include "vapor/input/Mouse.h"
#include "vapor/input/Keyboard.h"
#include "vapor/input/Joystick.h"

namespace vapor { namespace input {

//-----------------------------------//

/**
 * Input manager implementation that handles the events provided by
 * the SFML library. The SFML main event loop in the window class 
 * will just call this to process all the input-related events.
 */

class SFML_InputManager : public InputManager
{
public:

	SFML_InputManager();
	virtual ~SFML_InputManager();

	/// Feeds a SFML input event to the input manager.
	void processSFMLEvent( const sf::Event& event );

private:

	bool isMouseEvent(sf::Event::EventType eventType);
	bool isKeyboardEvent(sf::Event::EventType eventType);
	bool isJoystickEvent(sf::Event::EventType eventType);

	void processMouseEvent(const sf::Event& event);
	void processKeyboardEvent(const sf::Event& event);
	void processJoystickEvent(const sf::Event& event);

	Keys::Enum convertKeyEnum(sf::Key::Code keycode);
	MouseButton::Enum convertMouseButtonEnum(sf::Mouse::Button mouseButton);
	JoystickAxis::Enum convertJoystickAxisEnum(sf::Joy::Axis joyAxis);
};

//-----------------------------------//

} } // end namespaces

#endif