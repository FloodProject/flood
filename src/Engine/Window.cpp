/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/render/Window.h"

#if defined(VAPOR_WINDOWING_SDL)
	#include <vapor/render/SDL_Window.h>
#elif defined(VAPOR_WINDOWING_WIN32)
	#include <vapor/render/Win32_Window.h>
#elif defined(VAPOR_WINDOWING_SFML)
	#include <vapor/render/SFML_Window.h>
#else
	#error "OpenGL renderer needs a windowing implementation"
#endif

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

Window::Window(const Settings& settings)
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

Window& Window::createWindow( const Settings& settings )
{
	Window* window;

	#if defined(VAPOR_WINDOWING_SDL)
		window = new SDLWindow(settings);
	#elif defined(VAPOR_WINDOWING_WIN32)
		window = new Win32Window(settings);
	#elif defined(VAPOR_WINDOWING_SFML)
		window = new SFML_Window( const_cast< Settings& >( settings ) );
	#else
		#error "Could not find a window implementation"
	#endif

	return *window;
}


//-----------------------------------//

} } // end namespaces

