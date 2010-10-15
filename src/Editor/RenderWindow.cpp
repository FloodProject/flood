/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "RenderWindow.h"
#include "EditorInputManager.h"

namespace vapor { namespace editor {

//-----------------------------------//

RenderWindow::RenderWindow(const WindowSettings& settings,
						 wxGLCanvas* const canvas)
	: Window(settings)
	, canvas(canvas)
	, context(nullptr)
{
	createContext();
	inputManager = new EditorInputManager();
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
	if(!canvas)
		return false;

    // create OpenGL context
    context = new wxGLContext(canvas);
	
	if(!context)
	{
		Log::error("Error creating wxGLCanvas context");
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
	canvas->WarpPointer( x, y );
}

//-----------------------------------//

bool RenderWindow::isCursorVisible() const
{
	return !canvas->HasCapture();
}

//-----------------------------------//

void RenderWindow::setCursorVisible(bool mouseVisible)
{
	if( !mouseVisible )
	{
		canvas->SetCursor( wxCursor(wxCURSOR_BLANK) );
		canvas->CaptureMouse();
	}
	else
	{
		canvas->SetCursor( wxNullCursor );
		canvas->ReleaseMouse();
	}
}

//-----------------------------------//

void RenderWindow::processResize(const wxSize& size)
{
	settings.setWidth( size.GetX() );
	settings.setHeight( size.GetY() );

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