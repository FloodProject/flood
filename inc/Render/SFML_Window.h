/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#ifdef VAPOR_WINDOWING_SFML

#include "Render/Window.h"
#include "input/SFML_InputManager.h"
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

	// Swaps the buffers (updates the display).
	void update();

	// Shows/hides the window.
	virtual void show( bool hide = false );

	// Handle the window message events.
	bool pumpEvents();

	// Sets the title of the window.
	void setTitle(const std::string& title);

	// Sets the visibility of the mouse cursor.
	void setCursorVisible(bool state);

	// Gets the visibility of the mouse cursor.
	virtual bool isCursorVisible() const;

	// Gets the cursor position on screen.
	virtual Vector2 getCursorPosition() const;

	// Sets the cursor position on screen.
	virtual void setCursorPosition( int x, int y );

	// Marks this as the current context.
	virtual void makeCurrent();

	// Gets the input manager.
	InputManager* getInputManager() { return &inputManager; }

private:

	// Opens a new window.
	bool open();

	// Processes the different kinds of events.
	void processResize(sf::Event event);
	void processInput(sf::Event event);

	// Handles the creation of the window.
	void createWindow();

	// Holds the SFML input manager.
	SFML_InputManager inputManager;

	// SFML structures
	sf::Window window;
	sf::WindowSettings windowSettings;
	sf::VideoMode videoMode;
	uint32 flags;
	
	// Holds the current state of the cursor.
	bool cursorState;
};

//-----------------------------------//

NAMESPACE_ENGINE_END

#endif