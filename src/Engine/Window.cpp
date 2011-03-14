/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"
#include "Render/Window.h"
#include "input/Keyboard.h"
#include "Engine.h"

#if defined(VAPOR_WINDOWING_WIN32)
	#include <Render/Win32_Window.h>
#elif defined(VAPOR_WINDOWING_SFML)
	#include <Render/SFML_Window.h>
#else
	//#error "OpenGL renderer needs a windowing implementation"
#endif

namespace vapor {

//-----------------------------------//

WindowSettings::WindowSettings( const uint16 width, const uint16 height, 
	const std::string& title, const bool fullscreen )
	: Settings( width, height )
	, title(title)
	, fullScreen(fullscreen)
	, bitsPerPixel(32)
	, stencilBits(8)
	, depthBits(24)
	, antialiasLevel(0)
	, handle(nullptr)
{ }

//-----------------------------------//

Window::Window(const WindowSettings& settings)
	: settings(settings)
{
	LogInfo( "Creating %swindow (size: %dx%d, title: '%s', bits-per-pixel: %d)",
		settings.fullScreen ? "fullscreen " : "",
		settings.width, settings.height,
		settings.title.c_str(), settings.bitsPerPixel );

	if( settings.handle )
	{
		LogInfo( "External window handle found" );
	}
}

//-----------------------------------//

void Window::handleWindowResize()
{
	LogInfo("Resizing window to size %dx%d", settings.width, settings.height );
	onTargetResize( getSettings() );
}

//-----------------------------------//

void Window::handleWindowClose()
{
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
		Keyboard* keyboard = GetEngine()->getInputManager()->getKeyboard();

		if(keyboard)
			keyboard->resetKeys();
	}

	onWindowFocusChange( focusLost );
}

//-----------------------------------//

void Window::setCursorPosition( const Vector2& pos )
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