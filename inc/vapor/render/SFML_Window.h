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

namespace vapor {
	namespace render {

class SFMLWindow : public Window
{
public:

	SFMLWindow(Settings& settings);
	virtual ~SFMLWindow();

	// Swaps the buffers (updates the display)
	void update();

	// Handle the window message events
	bool pumpEvents();

	// Sets the title of the window
	void setTitle(const std::string& title);

	// Sets the cursor visibility
	void setCursor(bool state);

	void makeCurrent();

private:


	// Opens a new window
	bool open();
	void processResize(sf::Event event);
	sf::Window window;
	sf::WindowSettings sfmlSettings;
	sf::VideoMode vMode;
	unsigned long flags;


};

} } // end namespaces

#endif