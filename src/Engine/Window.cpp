/************************************************************************
*
* vaporEngine by triton © (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/render/Window.h"

namespace vapor {
	namespace render {

//-----------------------------------//

Settings::Settings(const int width, const int height,
	const int bpp, const bool fullscreen)
	: width(width), height(height), bpp(bpp), fullscreen(fullscreen)
{

}

//-----------------------------------//

Window::Window(Settings& settings)
	: settings(&settings)
{
	info("render::window", "Creating %dx%d window",
		getSettings().getWidth(), getSettings().getHeight());
}

//-----------------------------------//

Window::~Window()
{
	//delete settings;
}

//-----------------------------------//

Settings& Window::getSettings()
{
	return *settings;
}

//-----------------------------------//

} } // end namespaces
