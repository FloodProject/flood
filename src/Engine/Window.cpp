/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

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

namespace vapor { namespace render {

//-----------------------------------//

WindowSettings::WindowSettings(const ushort width, const ushort height, const bool fullscreen,
	const std::string title, const ushort bpp, const ushort depthbits, 
	const ushort stencilbits, const ushort aalevel,  void* customHandle)
	: Settings( width, height ), title(title), bpp(bpp), 
	depthbits(depthbits),  stencilbits(stencilbits), aalevel(aalevel),
	fullscreen(fullscreen), customHandle(customHandle)
{

}

//-----------------------------------//

Window::Window(const WindowSettings& settings)
	: settings( settings )
{
	info( "render::window", 
		"Creating %swindow (size: %dx%d, title: '%s', bits-per-pixel: %d)",
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

const Settings& Window::getSettings()
{
	return settings;
}

//-----------------------------------//

void Window::handleWindowResize()
{
	if( onTargetResize.empty() )
	{
		return;
	}

	const Settings& settings = getSettings();

	onTargetResize( settings );
}

//-----------------------------------//

void Window::handleWindowClose()
{
	if( onWindowClose.empty() )
	{
		return;
	}

	onWindowClose();
}

//-----------------------------------//

Window& Window::createWindow( const WindowSettings& settings )
{
	Window* window;

	#if defined(VAPOR_WINDOWING_SDL)
		window = new SDLWindow(settings);
	#elif defined(VAPOR_WINDOWING_WIN32)
		window = new Win32Window(settings);
	#elif defined(VAPOR_WINDOWING_SFML)
		window = new SFML_Window( const_cast< WindowSettings& >( settings ) );
	#else
		#error "Could not find a window implementation"
	#endif

	return *window;
}

//-----------------------------------//

} } // end namespaces