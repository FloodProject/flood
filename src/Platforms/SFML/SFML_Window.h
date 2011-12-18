/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#define ENABLE_WINDOW_SFML

#ifdef ENABLE_WINDOW_SFML

#include "Window/Window.h"
#include "SFML_Input.h"
#include <SFML/Window.hpp>

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

/**
 * Window implementation using the SFML library.
 */

class SFML_Window : public Window
{
public:

	SFML_Window(const WindowSettings& settings);
	virtual ~SFML_Window();

	// Opens a new window.
	bool open();

	// Swaps the buffers (updates the display).
	void update() OVERRIDE;

	// Shows/hides the window.
	void show( bool hide = false ) OVERRIDE;

	// Handle the window message events.
	bool pumpEvents() OVERRIDE;

	// Sets the title of the window.
	void setTitle(const String& title) OVERRIDE;

	// Sets the visibility of the mouse cursor.
	void setCursorVisible(bool state) OVERRIDE;

	// Gets the visibility of the mouse cursor.
	bool isCursorVisible() const OVERRIDE;

	// Gets the cursor position on screen.
	Vector2i getCursorPosition() const OVERRIDE;

	// Sets the cursor position on screen.
	void setCursorPosition( int x, int y ) OVERRIDE;

	// Marks this as the current context.
	void makeCurrent() OVERRIDE;

	// Gets the input manager.
	InputManager* getInput() OVERRIDE { return &input; }

private:

	// Processes the different kinds of events.
	void processResize(sf::Event event);
	void processInput(sf::Event event);

	// Handles the creation of the window.
	void createWindow();

	// Holds the SFML input manager.
	SFML_Input input;

	// SFML structures
	sf::Window window;
	sf::ContextSettings contextSettings;
	sf::VideoMode videoMode;
	uint32 flags;
	
	// Holds the current state of the cursor.
	bool cursorState;
};

//-----------------------------------//

NAMESPACE_ENGINE_END

#endif