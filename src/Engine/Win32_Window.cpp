/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/render/Win32_Window.h"

#ifdef VAPOR_WINDOWING_WIN32

namespace vapor {
	namespace render {

//-----------------------------------//

Win32Window::Win32Window(Settings& settings)
	:	Window(settings)
{
	if ( !create() ) {
		error("render::window", "Could not open a Win32 window");
		//exit(1);
	}
}

//-----------------------------------//

Win32Window::~Win32Window()
{

}

//-----------------------------------//

bool Win32Window::create()
{
return false;
}

//-----------------------------------//

void Win32Window::update()
{

}

//-----------------------------------//

bool Win32Window::pumpEvents()
{
return false;
}

//-----------------------------------//

void Win32Window::setTitle(const string& title) const
{

}


//-----------------------------------//

void Win32Window::setCursor(bool state) const
{

}


//-----------------------------------//

void Win32Window::makeCurrent()
{

}

//-----------------------------------//

} } // end namespaces

#endif