/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"
#include "Render/Window.h"
#include "Render/Device.h"
#include "Render/RenderContext.h"
#include "Input/Keyboard.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

WindowSettings::WindowSettings( uint16 width, uint16 height, const String& title, bool fullscreen )
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
	LogInfo( "Creating render window (size: %dx%d, title: '%s')",
		settings.width, settings.height, settings.title.c_str() );
}

//-----------------------------------//

void Window::handleWindowResize()
{
	//LogInfo("Resizing window to size %dx%d", settings.width, settings.height );
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
		GetInputManager()->setWindow(nullptr);
	else
		GetInputManager()->setWindow(this);

	onWindowFocusChange( focusLost );
}

//-----------------------------------//

void Window::setCursorPosition( const Vector2i& pos )
{
	setCursorPosition( pos.x, pos.y );
}

//-----------------------------------//

NAMESPACE_ENGINE_END