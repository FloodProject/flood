/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/render/sdl/SDL_Window.h"

#ifdef VAPOR_WINDOWING_SDL

#ifdef VAPOR_RENDERER_OPENGL
	#include "vapor/render/GL.h"

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
	:	Window(settings)
{
	if ( !init() || !open() ) {
		exit(1);
	}
}

//-----------------------------------//

SDLWindow::~SDLWindow()
{
	if(context) 
		SDL_GL_DeleteContext(context);
	
	if(windowId)
		SDL_DestroyWindow(windowId);

	// shutdown SDL
	SDL_VideoQuit();

	//SDL_Quit();
}

//-----------------------------------//

bool SDLWindow::init(void)
{
	info("render::window::sdl", "Initializing SDL subsystem");

	// initialize video sub-system
	if ( SDL_VideoInit(nullptr, 0) < 0 ) {
		error("render::window::sdl", 
			"Failed to initialize SDL %s", SDL_GetError());
		return false;
	}

	info("render::window::sdl", "Using SDL version %d.%d.%d", 
		SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_PATCHLEVEL);

	return true;
}

//-----------------------------------//

bool SDLWindow::open()
{
	Uint32 flags = 0;

	#ifdef VAPOR_RENDERER_OPENGL
		flags |= SDL_WINDOW_OPENGL;
	#endif

	// check for an external window handle
	if (settings.getCustomHandle() != nullptr)
	{
		void* handle = settings.getCustomHandle();
		windowId = SDL_CreateWindowFrom(handle);

	}
	else
	{
		if(settings.isFullscreen()) {
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		flags |= SDL_WINDOW_SHOWN;

		windowId = SDL_CreateWindow("vaporEngine", 
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
			settings.getWidth(), settings.getHeight(), flags);
	}

	if(!windowId)
	{
		error("render::window::sdl", 
			"Error creating SDL window: %s", SDL_GetError());
		return false;
	}

	//SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
	//SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
	//SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
	//SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 5 );
	//SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    // create OpenGL context
    context = SDL_GL_CreateContext(windowId);
	
	if(!context)
	{
		error("render::window::sdl", 
			"Error creating OpenGL context: %s", SDL_GetError());
		return false;
	}

	return true;
}

//-----------------------------------//

void SDLWindow::update() 
{
	if(!context) return;

	#ifdef VAPOR_RENDERER_OPENGL
		// swap buffers and update window
		SDL_GL_SwapWindow(windowId);
	#else
		#error "SDL needs window buffer swapping implementation."
	#endif
}

//-----------------------------------//

void SDLWindow::makeCurrent()
{
	if(!context || !windowId) return;

	SDL_GL_MakeCurrent(windowId, context);
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
	SDL_SetWindowTitle(windowId, title.c_str());
	
	info("render::window::sdl", 
		"Changing window title to '%s'", title.c_str());
}

//-----------------------------------//

void SDLWindow::setCursor(bool state) const
{
	SDL_ShowCursor(state);
}

} } // end namespaces

#endif