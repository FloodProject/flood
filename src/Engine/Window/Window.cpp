/************************************************************************
*
* Flood Project � (2008-201x)
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

WindowSettings::WindowSettings()
	: Settings(0, 0)
	, styles(WindowStyles::None)
	, handle(nullptr)
{

}

//-----------------------------------//

WindowSettings::WindowSettings(uint16 width, uint16 height, const String& title,
		WindowStyles styles)
	: Settings(width, height)
	, title(title)
	, styles(styles)
	, handle(nullptr)
{

}

//-----------------------------------//

Window::Window(const WindowSettings& settings)
	: settings(settings)
{
	LogInfo( "Creating window (size: %dx%d, title: '%s')",
		settings.width, settings.height, settings.title.CString() );
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
	onWindowFocusChange( focusLost );
}

//-----------------------------------//

void Window::setCursorPosition(int x, int y)
{
}

//-----------------------------------//

NAMESPACE_ENGINE_END