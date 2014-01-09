/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "API.h"
#include "WxWindow.h"
#include "WxRenderContext.h"
#include "WxPlatform.h"
#include <wx/glcanvas.h>

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

WxWindow::WxWindow(const WindowSettings& settings, wxWindow* window,
	WxInputManager* inputManager)
	: Window(settings)
	, window(window)
	, canvas(0)
	, cursorVisible(true)
	, cursorPriority(0)
	, inputManager(inputManager)
{
	setUserData(this);
	window->SetClientData(this);
	window->Bind(wxEVT_IDLE, &WxWindow::processIdle, this);

	canvas = new WxGLCanvas(window);
	canvas->SetSize(window->GetSize());
	canvas->input = inputManager;
	canvas->startFrameLoop();
}

//-----------------------------------//

WxWindow::~WxWindow()
{
	LogDebug("Destroying WxWindow");
	removeViews();

	LogDebug("Destroying RenderContext");
	context.reset();
}

//-----------------------------------//

RenderContext* WxWindow::createContext(const RenderContextSettings& settings)
{
	if (!window) return false;

	context = AllocateThis(WxRenderContext, canvas);
	return context.get();
}

//-----------------------------------//

void WxWindow::update() 
{
	if (!context) return;

	// Swap buffers and update window content.
	canvas->SwapBuffers();
}

//-----------------------------------//

void WxWindow::show( bool hide ) 
{
	window->Show( hide );
}

//-----------------------------------//

void WxWindow::makeCurrent()
{
	if( !context || !canvas ) 
		return;

	((WxRenderContext*)context.get())->makeCurrent(this);
}

//-----------------------------------//

bool WxWindow::hasFocus()
{
	return canvas->HasFocus();
}

//-----------------------------------//

Vector2i WxWindow::getCursorPosition() const
{
	const wxMouseState& mouseState = wxGetMouseState();
	
	int x = mouseState.GetX();
	int y = mouseState.GetY();
	canvas->ScreenToClient( &x, &y );
	
	return Vector2i(x, y);
}

//-----------------------------------//

void WxWindow::setCursorPosition( int x, int y )
{
	canvas->WarpPointer(x, y);
}

//-----------------------------------//

bool WxWindow::isCursorVisible() const
{
	return cursorVisible;
}

//-----------------------------------//

void WxWindow::setCursorVisible(bool state)
{
	cursorVisible = state;

	if( !cursorVisible )
		canvas->SetCursor( wxCursor(wxCURSOR_BLANK) );
	else
		canvas->SetCursor( wxNullCursor );
}

//-----------------------------------//

void WxWindow::setCursorVisiblePriority(bool state, int32 priority)
{
	if(cursorPriority > priority)
		return;

	setCursorVisible(state);
	
	if( !state )
		cursorPriority = 0;
}

//-----------------------------------//

void WxWindow::setCursorCapture( bool captureMouse )
{
	bool hasCapture = canvas->HasCapture();

	if(captureMouse)
		canvas->CaptureMouse();
	else if(hasCapture)
		canvas->ReleaseMouse();
}

//-----------------------------------//

void WxWindow::processResize(const wxSize& size)
{
	settings.width = size.GetX();
	settings.height = size.GetY();

	handleWindowResize();
}

//-----------------------------------//

void WxWindow::processIdle(wxIdleEvent& event)
{
	onIdle();
	//event.RequestMore();
}

//-----------------------------------//

bool WxWindow::pumpEvents()
{
	return true;
}

//-----------------------------------//

void WxWindow::setTitle(const String& title)
{
	// Our canvas has no title to set, the best we can do is to
	// set the title as the help text of the wxWidgets control.
    canvas->SetHelpText( title.CString() );
}

//-----------------------------------//

NAMESPACE_EDITOR_END