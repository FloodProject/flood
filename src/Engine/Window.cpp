/************************************************************************
*
* vaporEngine by triton (2008)
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

#include "vapor/Engine.h"
#include "vapor/render/Window.h"

namespace vapor {
	namespace render {

WindowSettings::WindowSettings(const int width, const int height,
	const int bpp, const bool fullscreen)
	: width(width), height(height), bpp(bpp), fullscreen(fullscreen)
{

}

Window::Window(WindowSettings *windowSettings)
: _windowSettings(windowSettings)
{
	_windowSettings = windowSettings;

	info("render::window", "Creating %dx%d window",
		getWindowSettings().width, getWindowSettings().height);
}

Window::~Window()
{
	delete _windowSettings;
}

WindowSettings& Window::getWindowSettings()
{
	return *_windowSettings;
}

} } // end namespaces
