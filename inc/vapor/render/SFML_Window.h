/************************************************************************
*
* vaporEngine (2008-2009)
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

namespace vapor {
	namespace render {

/**
 * Window implementation using the SFML library.
 */

class SFML_Window : public Window
{
public:

	SFML_Window(const Settings& settings);
	virtual ~SFML_Window();

	// Swaps the buffers (updates the display).
	void update();

	// Shows/hides the window.
	virtual void show( bool hide = false );

	// Handle the window message events.
	bool pumpEvents();

	// Sets the title of the window.
	void setTitle(const std::string& title);

	// Sets the cursor visibility.
	void setCursor(bool state);

	// Marks this as the current context.
	void makeCurrent();

	// Gets the input manager.
	input::SFML_InputManager& getInputManager() { return inputManager; }

private:

	// Opens a new window.
	bool open();

	void processResize(sf::Event event);
	void processInput(sf::Event event);

	void createWindow();

	input::SFML_InputManager inputManager;
	
	sf::Window window;
	sf::WindowSettings sfmlSettings;
	sf::VideoMode vMode;
	unsigned long flags;
};

} } // end namespaces

#endif