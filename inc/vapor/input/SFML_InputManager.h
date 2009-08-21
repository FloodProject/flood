/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/CompileOptions.h"

#ifdef VAPOR_INPUT_SFML

#include <SFML/Window.hpp>

#include "vapor/input/InputManager.h"

namespace vapor {
	namespace input {

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

	// Feeds an SFML input event to the input manager.
	void processEvent( const sf::Event& event );
};

//-----------------------------------//

} } // end namespaces

#endif