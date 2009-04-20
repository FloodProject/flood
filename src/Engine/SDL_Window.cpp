/************************************************************************
*
* vaporEngine by triton © (2008-2009)
*
*	<http://www.portugal-a-programar.org/
*
************************************************************************/

#include <GL/glew.h>
#include "vapor/render/SDL_Window.h"

#ifdef VAPOR_WINDOWING_SDL

namespace vapor {
	namespace render {

SDLWindow::SDLWindow(WindowSettings *windowSettings)
	:	Window(windowSettings)
{
	if ( !init() || !open() ) {
		exit(1);
	}
}

SDLWindow::~SDLWindow()
{
	// shutdown SDL
	SDL_Quit();
}

bool SDLWindow::init(void)
{
	display = nullptr;

	info("render::window", "Initializing SDL subsystem");

	// initialize video sub-system
	if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
		error("render::window", "Failed to initialize SDL");
		return false;
	}

	return true;
}

bool SDLWindow::open()
{
	Uint32 flags = SDL_OPENGL;

	// check if we want fullscreen
	if ( getWindowSettings().fullscreen )
		flags |= SDL_FULLSCREEN;

	//SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
	//SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
	//SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
	//SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 5 );
	//SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

	// set the video mode
	display = SDL_SetVideoMode(getWindowSettings().width,
		getWindowSettings().height, getWindowSettings().bpp, flags);

	glewInit();

	if ( !display ) {
		error("render::window", "Failed to create a display");
		return false;
	}

	return true;
}

void SDLWindow::update() 
{
	// update the screen
	SDL_GL_SwapBuffers();
}

bool SDLWindow::pump()
{
	SDL_Event event;

	while( SDL_PollEvent(&event) )
	{
		switch ( event.type )
		{
		case SDL_KEYDOWN:
			if ( event.key.keysym.sym == SDLK_ESCAPE )
				return false;
			break;
		case SDL_QUIT:
				return false;
				break;
		}
	}

	SDL_Delay(1);

	return true;
}

void SDLWindow::setTitle(const string& title) const
{
	SDL_WM_SetCaption(title.c_str(), NULL);
	info("render::window", "Changing window title to '%s'", title.c_str());
}

void SDLWindow::setCursor(bool state) const
{
	SDL_ShowCursor(state);
}

} } // end namespaces

#endif