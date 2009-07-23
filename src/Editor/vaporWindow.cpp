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

vaporWindow::vaporWindow(render::Settings& settings, wxGLCanvas* canvas)
	:	render::Window(settings), canvas(canvas), context(nullptr)
{
	open();
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

void vaporWindow::setTitle(const string& title) const
{

}

//-----------------------------------//

void vaporWindow::setCursor(bool state) const
{

}

//} } // end namespaces

//#endif