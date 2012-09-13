/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Engine/API.h"
#include "Engine/Window/Window.h"
#include "Graphics/RenderDevice.h"
#include "Graphics/RenderContext.h"
#include "Engine/Input/Keyboard.h"

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
	//LogInfo("Resizing window to size %dx%d", settings.width, settings.height);
	handleResize();
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