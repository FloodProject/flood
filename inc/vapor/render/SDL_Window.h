/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/CompileOptions.h"

#ifdef VAPOR_WINDOWING_SDL

#if VAPOR_WINDOWING_SDL == 13
	#define SDL_NO_COMPAT
#endif

#include <SDL.h>

#include "vapor/Platform.h"
#include "vapor/render/Window.h"

namespace vapor { namespace render {

//-----------------------------------//

class SDLWindow : public Window
{
public:

	SDLWindow(Settings& settings);
	virtual ~SDLWindow();

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

#if VAPOR_WINDOWING_SDL == 12
	// Display surface
	SDL_Surface	*display;	
#else
	// SDL window id
	SDL_WindowID windowId;

	// OpenGL context
	SDL_GLContext context;
#endif

	// Initializes SDL
	bool init();

	// Opens a new window
	bool open();
};

//-----------------------------------//

} } // end namespaces

#endif