/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/render/Window.h"

namespace vapor {
	namespace render {

//-----------------------------------//

Settings::Settings(const int width, const int height,
	const int bpp, const bool fullscreen, void* customHandle)
	: width(width), height(height), bpp(bpp), 
	fullscreen(fullscreen), customHandle(customHandle)
{

}

//-----------------------------------//

Window::Window(Settings& settings)
	: settings(settings)
{
	info("render::window", "Creating %dx%d window",
		settings.getWidth(), settings.getHeight());

	if(settings.getCustomHandle())
	{
		info("render::window", "External window handle found: %d",
			settings.getCustomHandle());
	}
}

//-----------------------------------//

Window::~Window()
{
	//delete settings;
}

//-----------------------------------//

Settings& Window::getSettings()
{
	return settings;
}

//-----------------------------------//

} } // end namespaces
