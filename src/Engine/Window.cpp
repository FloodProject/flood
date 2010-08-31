/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/render/Window.h"
#include "vapor/input/Keyboard.h"
#include "vapor/render/View.h"

#if defined(VAPOR_WINDOWING_WIN32)
	#include <vapor/render/Win32_Window.h>
#elif defined(VAPOR_WINDOWING_SFML)
	#include <vapor/render/SFML_Window.h>
#else
	//#error "OpenGL renderer needs a windowing implementation"
#endif

namespace vapor {

//-----------------------------------//

WindowSettings::WindowSettings( const ushort width, const ushort height, 
	const std::string& title, const bool fullscreen )
	: Settings( width, height )
	, title(title)
	, fullScreen(fullscreen)
	, bitsPerPixel(32)
	, stencilBits(8)
	, depthBits(24)
	, antialiasLevel(0)
	, customHandle(nullptr)
{ }

//-----------------------------------//

WindowSettings::WindowSettings( const WindowSettings& s )
	: Settings(s)
	, title(s.title)
	, fullScreen(s.fullScreen)
	, bitsPerPixel(s.bitsPerPixel)
	, stencilBits(s.stencilBits)
	, depthBits(s.depthBits)
	, antialiasLevel(s.antialiasLevel)
	, customHandle(s.customHandle)
{ }

//-----------------------------------//

Window::Window(const WindowSettings& settings)
	: settings(settings)
{
	info( "window", "Creating %swindow \
		(size: %dx%d, title: '%s', bits-per-pixel: %d)",
		settings.isFullscreen() ? "fullscreen " : "",
		settings.getWidth(), settings.getHeight(),
		settings.getTitle().c_str(), settings.getBpp() );

	if( settings.getCustomHandle() )
	{
		info("window", "External window handle found: %d",
			settings.getCustomHandle());
	}
}

//-----------------------------------//

void Window::handleWindowResize()
{
	info("window", "Resizing window (new size: %dx%d)",
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
	if( focusLost )
	{
		// Workaround for resetting the pressed keys when the window
		// loses focus. This could lead to problems when the user
		// unfocus the main control. In that case the key events
		// might not be properly handled by the input implementation.
		Keyboard* keyboard = getInputManager()->getKeyboard();

		if(keyboard)
			keyboard->resetKeys();
	}

	if( onWindowFocusChange.empty() )
		return;

	onWindowFocusChange( focusLost );
}

//-----------------------------------//

void Window::setCursorPosition( const Vector2i& pos )
{
	setCursorPosition( pos.x, pos.y );
}

//-----------------------------------//

Window* Window::createWindow( const WindowSettings& settings )
{
	Window* window = nullptr;

	#if defined(VAPOR_WINDOWING_WIN32)
		window = new Win32Window(settings);
	#elif defined(VAPOR_WINDOWING_SFML)
		window = new SFML_Window(settings);
	#else
		//#error "Could not find a window implementation"
	#endif

	return window;
}

//-----------------------------------//

} // end namespace