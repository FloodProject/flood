/************************************************************************
*
* vaporEngine by triton © (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/Engine.h"
#include "vapor/render/Window.h"

namespace vapor {
	namespace render {

Settings::Settings(const int width, const int height,
	const int bpp, const bool fullscreen)
	: _width(width), _height(height), _bpp(bpp), _fullscreen(fullscreen)
{

}

Window::Window(Settings& settings)
	: _settings(&settings)
{
	info("render::window", "Creating %dx%d window",
		getSettings().width(), getSettings().height());
}

Window::~Window()
{
	//delete _settings;
}

Settings& Window::getSettings()
{
	return *_settings;
}

} } // end namespaces
