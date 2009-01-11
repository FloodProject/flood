/************************************************************************
*
* vaporEngine by triton © (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
* DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
* Version 2, December 2004
* 
* DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
* TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
* 
* 0. You just DO WHAT THE FUCK YOU WANT TO.
*
************************************************************************/

#pragma once

#include "vapor/CompileOptions.h"

#ifdef VAPOR_WINDOWING_SDL

#include <SDL.h>

#include "vapor/Engine.h"
#include "vapor/render/Window.h"

namespace vapor {
	namespace render {

class SDLWindow : public Window
{
public:

	SDLWindow(WindowSettings *windowSettings);
	virtual ~SDLWindow();

	void update ();

	bool pump();

	void setTitle(const string& title) const;

	void setCursor(bool state) const;

private:

	// Display surface
	SDL_Surface	*display;			

	// Initializes the Display system
	bool init();

	// Opens a new window
	bool open ();
};

} } // end namespaces

#endif