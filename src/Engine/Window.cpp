/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
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

WindowSettings::WindowSettings( const ushort width, const ushort height, 
	const std::string& title, const bool fullscreen )
	: Settings( width, height ), title(title), fullScreen(fullscreen), 
	bitsPerPixel(32), stencilBits(8), depthBits(24), antialiasLevel(0),
	customHandle(nullptr)
{

}

//-----------------------------------//

WindowSettings::WindowSettings( const WindowSettings& s )
	: Settings( s ), title(s.title), fullScreen(s.fullScreen), 
	bitsPerPixel(s.bitsPerPixel), stencilBits(s.stencilBits),
	depthBits(s.depthBits), antialiasLevel(s.antialiasLevel),
	customHandle(s.customHandle)
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

void Window::handleWindowResize()
{
	info("render::window", "Resizing window (new size: %dx%d)",
		settings.getWidth(), settings.getHeight() );

	if( onTargetResize.empty() )
		return;

	onTargetResize( getSettings() );
}

//-----------------------------------//

void Window::handleWindowClose()
{
	if( onWindowClose.empty() )
		return;

	onWindowClose();
}

//-----------------------------------//

void Window::handleWindowFocus( bool focusLost )
{
	if( onWindowFocusChange.empty() )
		return;

	onWindowFocusChange( focusLost );
}

//-----------------------------------//

void Window::setCursorPosition( const math::Vector2i& pos )
{
	setCursorPosition( pos.x, pos.y );
}

//-----------------------------------//

WindowPtr Window::createWindow( const WindowSettings& settings )
{
	WindowPtr window;

	#if defined(VAPOR_WINDOWING_SDL)
		window = new SDLWindow(settings);
	#elif defined(VAPOR_WINDOWING_WIN32)
		window = new Win32Window(settings);
	#elif defined(VAPOR_WINDOWING_SFML)
		window = new SFML_Window(settings);
	#else
		#error "Could not find a window implementation"
	#endif

	return window;
}

//-----------------------------------//

} } // end namespaces