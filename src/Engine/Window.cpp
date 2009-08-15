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

Settings::Settings(const int width, const int height, const std::string title,
	const int bpp, const int depthbits, const int stencilbits,
	const int aalevel, const bool fullscreen, void* customHandle)
	: width(width), height(height), title(title), bpp(bpp), 
	depthbits(depthbits),  stencilbits(stencilbits), aalevel(aalevel),
	fullscreen(fullscreen), customHandle(customHandle)
{

}

//-----------------------------------//

Window::Window(Settings& settings)
	: settings(settings)
{
	info( "render::window", "Creating %swindow (size: %dx%d, title: '%s', bits-per-pixel: %d)",
		settings.isFullscreen() ? "fullscreen " : "", settings.getWidth(), 
		settings.getHeight(), settings.getTitle().c_str(), settings.getBpp() );

	if( settings.getCustomHandle() )
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