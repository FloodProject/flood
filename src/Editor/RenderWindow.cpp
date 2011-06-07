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

namespace vapor { namespace editor {

//-----------------------------------//

RenderWindow::RenderWindow(const WindowSettings& settings, wxGLCanvas* const canvas)
	: Window(settings)
	, canvas(canvas)
	, context(nullptr)
	, cursorVisible(true)
	, cursorPriority(0)
{
	createContext();
	inputManager = new EditorInputManager(GetEngine()->getInputManager());
}

//-----------------------------------//

RenderWindow::~RenderWindow()
{
	delete context;
	delete inputManager;
}

//-----------------------------------//

bool RenderWindow::createContext()
{
	if(!canvas)return false;

    // create OpenGL context
    context = new wxGLContext(canvas);
	
	if(!context)
	{
		LogError("Error creating wxGLCanvas context");
		return false;
	}

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
	if(!context || !canvas) 
		return;

	context->SetCurrent(*canvas);
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

} } // end namespaces