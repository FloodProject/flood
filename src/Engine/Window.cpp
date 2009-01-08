/************************************************************************
*
* vaporEngine by triton (2008)
*
*	<http://www.portugal-a-programar.org>
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
