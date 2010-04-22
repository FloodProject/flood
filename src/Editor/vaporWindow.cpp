/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "vaporWindow.h"

namespace vapor { namespace editor {

//-----------------------------------//

vaporWindow::vaporWindow(const WindowSettings& settings,
						 wxGLCanvas* const canvas)
	: render::Window(settings), canvas(canvas), context(nullptr)
{
	createContext();
	im = new vaporInputManager();
}

//-----------------------------------//

vaporWindow::~vaporWindow()
{
	delete context;
	delete im;
}

//-----------------------------------//

bool vaporWindow::createContext()
{
	if(!canvas) return false;

    // create OpenGL context
    context = new wxGLContext(canvas);
	
	if(!context)
	{
		error("wx", "Error creating wxGLCanvas context");
		return false;
	}

	return true;
}

//-----------------------------------//

void vaporWindow::update() 
{
	if(!context) return;

	// Swap buffers and update window content.
	canvas->SwapBuffers();
}

//-----------------------------------//

void vaporWindow::show( bool hide ) 
{
	canvas->Show( hide );
}

//-----------------------------------//

void vaporWindow::makeCurrent()
{
	if(!context || !canvas) 
		return;

	context->SetCurrent(*canvas);
}

//-----------------------------------//

math::Vector2i vaporWindow::getCursorPosition() const
{
	const wxMouseState& mouseState = wxGetMouseState();
	
	int x = mouseState.GetX();
	int y = mouseState.GetY();
	canvas->ScreenToClient( &x, &y );
	
	return math::Vector2i(x, y);
}

//-----------------------------------//

void vaporWindow::setCursorPosition( int x, int y )
{
	canvas->WarpPointer( x, y );
}

//-----------------------------------//

bool vaporWindow::isCursorVisible() const
{
	return !canvas->HasCapture();
}

//-----------------------------------//

void vaporWindow::setCursorVisible(bool mouseVisible)
{
	if( !mouseVisible )
	{
		canvas->SetCursor( wxCursor( wxCURSOR_BLANK ) );
		canvas->CaptureMouse();
	}
	else
	{
		canvas->SetCursor( wxNullCursor );
		canvas->ReleaseMouse();
	}
}

//-----------------------------------//

void vaporWindow::processResize(const wxSize& size)
{
	settings.setWidth( size.GetX() );
	settings.setHeight( size.GetY() );

	handleWindowResize();
}

//-----------------------------------//

bool vaporWindow::pumpEvents()
{
	return true;
}

//-----------------------------------//

void vaporWindow::setTitle(const std::string& title)
{
	// Our canvas has no title to set, the best we can do is to
	// set the title as the help text of the wxWidgets control.
	canvas->SetHelpText( title );
}

//-----------------------------------//

} } // end namespaces