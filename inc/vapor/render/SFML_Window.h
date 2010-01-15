/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/CompileOptions.h"

#ifdef VAPOR_WINDOWING_SFML

#include <SFML/Window.hpp>

#include "vapor/Platform.h"
#include "vapor/render/Window.h"

#include "vapor/input/SFML_InputManager.h"

namespace vapor { namespace render {

//-----------------------------------//

/**
 * Window implementation using the SFML library.
 */

class SFML_Window : public Window
{
public:

	SFML_Window(const WindowSettings& settings);
	virtual ~SFML_Window();

	// Swaps the buffers (updates the display).
	void update();

	// Shows/hides the window.
	virtual void show( bool hide = false );

	// Handle the window message events.
	bool pumpEvents();

	// Sets the title of the window.
	void setTitle(const std::string& title);

	// Sets the visibility of the mouse cursor.
	void setCursorState(bool state);

	// Gets the visibility of the mouse cursor.
	virtual bool getCursorState() const;

	// Sets the cursor position on screen.
	virtual void setCursorPosition( int x, int y );

	// Marks this as the current context.
	virtual void makeCurrent();

	// Gets the input manager.
	input::SFML_InputManager& getInputManager() { return inputManager; }

private:

	// Opens a new window.
	bool open();

	// Processes the different kinds of events.
	void processResize(sf::Event event);
	void processInput(sf::Event event);

	// Handles the creation of the window.
	void createWindow();

	// Holds the SFML input manager.
	input::SFML_InputManager inputManager;

	// SFML structures
	sf::Window window;
	sf::WindowSettings sfmlSettings;
	sf::VideoMode vMode;
	unsigned long flags;
	
	// Holds the current state of the cursor.
	bool cursorState;
};

//-----------------------------------//

} } // end namespaces

#endif