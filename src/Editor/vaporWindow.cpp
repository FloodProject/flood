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

vaporWindow::vaporWindow(const render::WindowSettings& settings, wxGLCanvas* canvas)
	:	render::Window(settings), canvas(canvas), context(nullptr), mouseCaptured( false )
{
	open();

	im = new vaporInputManager();
}

//-----------------------------------//

vaporWindow::~vaporWindow()
{
	delete context;
	delete im;
}

//-----------------------------------//

bool vaporWindow::open()
{
	if(!canvas) return false;

    // create OpenGL context
    context = new wxGLContext(canvas);
	
	if(!context)
	{
		error("window::wx", "Error creating wxWidgets OpenGL context");
		return false;
	}

	return true;
}

//-----------------------------------//

void vaporWindow::update() 
{
	if(!context) return;

	// swap buffers and update window
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
	if(!context || !canvas) return;

	context->SetCurrent(*canvas);
}

//-----------------------------------//

bool vaporWindow::pumpEvents()
{
	return true;
}

//-----------------------------------//

void vaporWindow::setTitle(const std::string& UNUSED(title))
{

}

//-----------------------------------//

void vaporWindow::setCursorVisible(bool mouseVisible)
{
	if( !mouseVisible )
	{
		mouseCaptured = true;
		canvas->CaptureMouse();
		canvas->SetCursor( wxCursor( wxCURSOR_BLANK ) );
	}
	else
	{
		mouseCaptured = false;
		canvas->ReleaseMouse();
		canvas->SetCursor( wxNullCursor );
	}
}

//-----------------------------------//

bool vaporWindow::isCursorVisible() const
{
	return !mouseCaptured;
}

//-----------------------------------//

void vaporWindow::setCursorPosition( int x, int y )
{
	canvas->WarpPointer( x, y );
}

//-----------------------------------//

input::InputManager& vaporWindow::getInputManager()
{
	return *im;
}

//-----------------------------------//

void vaporWindow::processResize(const wxSize& size)
{
	settings.setWidth( size.GetX() );
	settings.setHeight( size.GetY() );

	handleWindowResize();
}

//-----------------------------------//

} } // end namespaces