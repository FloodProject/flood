/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"
#include "RenderWindow.h"
#include "EditorInputManager.h"
#include "Render/RenderContext.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

RenderWindow::RenderWindow(const WindowSettings& settings, wxGLCanvas* const canvas)
	: Window(settings)
	, canvas(canvas)
	, contextGL(nullptr)
	, cursorVisible(true)
	, cursorPriority(0)
{
	createContext();
	inputManager = new EditorInputManager(GetInputManager());
}

//-----------------------------------//

RenderWindow::~RenderWindow()
{
	LogDebug("Destroying RenderWindow");
	removeViews();

	LogDebug("Destroying RenderContext");
	Deallocate(context);

	LogDebug("Destroying OpenGL Context");
	delete contextGL;

	delete inputManager;
}

//-----------------------------------//

bool RenderWindow::createContext()
{
	if( !canvas ) return false;

    // create OpenGL context
    contextGL = new wxGLContext(canvas);
	
	if( !contextGL )
	{
		LogError("Error creating wxGLCanvas context");
		return false;
	}

	RenderContext* context = Allocate(RenderContext, GetRenderAllocator());
	setContext(context);

	return true;
}

//-----------------------------------//

void RenderWindow::update() 
{
	assert( context != nullptr );

	// Swap buffers and update window content.
	canvas->SwapBuffers();
}

//-----------------------------------//

void RenderWindow::show( bool hide ) 
{
	canvas->Show( hide );
}

//-----------------------------------//

void RenderWindow::makeCurrent()
{
	if( !contextGL || !canvas ) 
		return;

	contextGL->SetCurrent(*canvas);
}

//-----------------------------------//

bool RenderWindow::hasFocus()
{
	return canvas->HasFocus();
}

//-----------------------------------//

Vector2i RenderWindow::getCursorPosition() const
{
	const wxMouseState& mouseState = wxGetMouseState();
	
	int x = mouseState.GetX();
	int y = mouseState.GetY();
	canvas->ScreenToClient( &x, &y );
	
	return Vector2i(x, y);
}

//-----------------------------------//

void RenderWindow::setCursorPosition( int x, int y )
{
	canvas->WarpPointer(x, y);
}

//-----------------------------------//

bool RenderWindow::isCursorVisible() const
{
	return cursorVisible;
}

//-----------------------------------//

void RenderWindow::setCursorVisible(bool state)
{
	cursorVisible = state;

	if( !cursorVisible )
		canvas->SetCursor( wxCursor(wxCURSOR_BLANK) );
	else
		canvas->SetCursor( wxNullCursor );
}

//-----------------------------------//

void RenderWindow::setCursorVisiblePriority(bool state, int32 priority)
{
	if(cursorPriority > priority)
		return;

	setCursorVisible(state);
	
	if( !state )
		cursorPriority = 0;
}

//-----------------------------------//

void RenderWindow::setCursorCapture( bool captureMouse )
{
	bool hasCapture = canvas->HasCapture();

	if(captureMouse)
		canvas->CaptureMouse();
	else if(hasCapture)
		canvas->ReleaseMouse();
}

//-----------------------------------//

void RenderWindow::processResize(const wxSize& size)
{
	settings.width = size.GetX();
	settings.height = size.GetY();

	handleWindowResize();
}

//-----------------------------------//

bool RenderWindow::pumpEvents()
{
	return true;
}

//-----------------------------------//

void RenderWindow::setTitle(const std::string& title)
{
	// Our canvas has no title to set, the best we can do is to
	// set the title as the help text of the wxWidgets control.
	canvas->SetHelpText( title );
}

//-----------------------------------//

NAMESPACE_EDITOR_END