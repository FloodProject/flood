/************************************************************************
*
* vaporEngine by triton © (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/render/sdl/SDL_Window.h"

#ifdef VAPOR_WINDOWING_SDL

#ifdef VAPOR_RENDERER_OPENGL
	#include "vapor/render/gl/GL.h"

	// SDL extensions conflicts with GLEW
	#define NO_SDL_GLEXT
	#include <SDL_opengl.h>
#else
	#error "SDL is missing a renderer implementation"
#endif

namespace vapor {
	namespace render {

//-----------------------------------//

SDLWindow::SDLWindow(Settings& settings)
	:	Window(settings), display(nullptr)
{
	if ( !init() || !open() ) {
		exit(1);
	}
}

//-----------------------------------//

SDLWindow::~SDLWindow()
{
	// shutdown SDL
	SDL_Quit();
}

//-----------------------------------//

bool SDLWindow::init(void)
{
	info("render::window::sdl", "Initializing SDL subsystem");

	// initialize video sub-system
	if ( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE) < 0 ) {
		error("render::window::sdl", "Failed to initialize SDL");
		return false;
	}

	info("render::window::sdl", "Using SDL version %d.%d.%d", 
		SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_PATCHLEVEL);

	return true;
}

//-----------------------------------//

bool SDLWindow::open()
{
	Uint32 flags = SDL_HWPALETTE | SDL_RESIZABLE;

	#ifdef VAPOR_RENDERER_OPENGL
		flags |= SDL_OPENGL;
	#endif

	// check if we want fullscreen
	if (getSettings().isFullscreen())
		flags |= SDL_FULLSCREEN;

	//SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
	//SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
	//SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
	//SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 5 );
	//SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// set the video mode
	display = SDL_SetVideoMode(getSettings().getWidth(),
		getSettings().getHeight(), getSettings().getBpp(), flags);

	if ( !display ) {
		error("render::window::sdl", 
			"Failed to create a display: %s", SDL_GetError());
		return false;
	}

	return true;
}

//-----------------------------------//

void SDLWindow::update() 
{
	#ifdef VAPOR_RENDERER_OPENGL
		// swap buffers and update window
		SDL_GL_SwapBuffers();
	#else
		#error "SDL needs window buffer swapping implementation."
	#endif
}

//-----------------------------------//

bool SDLWindow::pumpEvents()
{
	SDL_Event event;

	while(SDL_PollEvent(&event))
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

	SDL_Delay(0);

	return true;
}

//-----------------------------------//

void SDLWindow::setTitle(const string& title) const
{
	SDL_WM_SetCaption(title.c_str(), NULL);
	info("render::window::sdl", "Changing window title to '%s'", title.c_str());
}

//-----------------------------------//

void SDLWindow::setCursor(bool state) const
{
	SDL_ShowCursor(state);
}

} } // end namespaces

#endif