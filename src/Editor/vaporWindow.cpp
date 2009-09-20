/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vaporWindow.h"

using namespace vapor;

//#ifdef VAPOR_WINDOWING_SDL

//namespace vapor {
//	namespace render {

//-----------------------------------//

vaporWindow::vaporWindow(const render::WindowSettings& settings, wxGLCanvas* canvas)
	:	render::Window(settings), canvas(canvas), context(nullptr)
{
	open();

	im = new wx_InputManager();
}

//-----------------------------------//

vaporWindow::~vaporWindow()
{
	delete context;
}

//-----------------------------------//

bool vaporWindow::open()
{
	if(!canvas) return false;

    // create OpenGL context
    context = new wxGLContext(canvas);
	
	if(!context)
	{
		error("render::window::sdl", 
			"Error creating wxWidgets OpenGL context");
		return false;
	}

	return true;
}

//-----------------------------------//

void vaporWindow::update() 
{
	if(!context) return;

	makeCurrent();

	// swap buffers and update window
	canvas->SwapBuffers();
}

//-----------------------------------//

void vaporWindow::show( bool UNUSED(hide) ) 
{

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

void vaporWindow::setCursor(bool UNUSED(state))
{

}

//-----------------------------------//

input::InputManager& vaporWindow::getInputManager()
{
	return *im;
}

//-----------------------------------//

//} } // end namespaces

//#endif