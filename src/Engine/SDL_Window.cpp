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
	:	Window(settings),

#if VAPOR_WINDOWING_SDL == 12
	display(nullptr)
#else
	windowId(0), context(nullptr) 
#endif

{
	if ( !init() || !open() ) {
		exit(1);
	}
}

//-----------------------------------//

SDLWindow::~SDLWindow()
{
#if VAPOR_WINDOWING_SDL == 12
	// shutdown SDL
	SDL_Quit();
#else
	if(context) {
		SDL_GL_DeleteContext(context);
	}
	
	if(windowId) {
		SDL_DestroyWindow(windowId);
	}

	// shutdown SDL
	SDL_VideoQuit();
#endif
}

//-----------------------------------//

bool SDLWindow::init(void)
{
	info("render::window::sdl", "Initializing SDL subsystem");

	// initialize video sub-system
#if VAPOR_WINDOWING_SDL == 12
	if ( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE) < 0 ) {
#else
	if ( SDL_VideoInit(nullptr, 0) < 0 ) {
#endif
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
	#if VAPOR_WINDOWING_SDL == 12
		Uint32 flags = SDL_HWPALETTE | SDL_RESIZABLE;
	#else
		Uint32 flags = 0;
	#endif

	#ifdef VAPOR_RENDERER_OPENGL
		#if VAPOR_WINDOWING_SDL == 12
			flags |= SDL_OPENGL;
		#else
			flags |= SDL_WINDOW_OPENGL;
		#endif
	#endif

	// check for an external window handle
	if (settings.getCustomHandle() != nullptr)
	{
		#if VAPOR_WINDOWING_SDL == 12
			// SDL 1.2 can receive a custom window id via an 
			// environment variable, smells like 'hack' to me :D

			#define BUF_SIZE 64

			#ifdef VAPOR_PLATFORM_WINDOWS
				#define snprintf sprintf_s
			#endif 
			
			char tmp[BUF_SIZE];
			ulong handle = (ulong) getSettings().getCustomHandle();
			snprintf(tmp, BUF_SIZE, "SDL_WINDOWID=%u", handle);
			
			#ifdef VAPOR_PLATFORM_WINDOWS
				int ret = _putenv(tmp);
			#else
				int ret = putenv(tmp);	
			#endif

			if(ret != 0)
			{
				error("render::window::sdl", "Error setting custom window handle");
			}
		#else
			void* handle = settings.getCustomHandle();
			windowId = SDL_CreateWindowFrom(handle);
		#endif
	}
	else
	{
		if(settings.isFullscreen()) {
			
			#if VAPOR_WINDOWING_SDL == 12
				flags |= SDL_FULLSCREEN;
			#else
				flags |= SDL_WINDOW_FULLSCREEN;
			#endif
		}

		#if VAPOR_WINDOWING_SDL == 13
			flags |= SDL_WINDOW_SHOWN;
		#endif

		#if VAPOR_WINDOWING_SDL == 12
			SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

			// set the video mode
			display = SDL_SetVideoMode(getSettings().getWidth(),
				getSettings().getHeight(), getSettings().getBpp(), flags);

			if ( !display ) {
				error("render::window::sdl", 
					"Failed to create a display: %s", SDL_GetError());
				return false;
			}
		#else
			windowId = SDL_CreateWindow("vaporEngine", 
				SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
				settings.getWidth(), settings.getHeight(), flags);
		#endif
	}

	#if VAPOR_WINDOWING_SDL == 13
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
	#endif

	return true;
}

//-----------------------------------//

void SDLWindow::update() 
{
#if VAPOR_WINDOWING_SDL == 13
	if(!context) return;
#endif

#ifdef VAPOR_RENDERER_OPENGL
	// swap buffers and update window
	#if VAPOR_WINDOWING_SDL == 12
		SDL_GL_SwapBuffers();
	#else
		SDL_GL_SwapWindow(windowId);
	#endif
#else
	#error "SDL needs window buffer swapping implementation."
#endif
}

//-----------------------------------//

void SDLWindow::makeCurrent()
{
#if VAPOR_WINDOWING_SDL == 13
	if(!context || !windowId) return;

	#ifdef VAPOR_RENDERER_OPENGL
		SDL_GL_MakeCurrent(windowId, context);
	#endif
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
#if VAPOR_WINDOWING_SDL == 12
	SDL_WM_SetCaption(title.c_str(), nullptr);
#else
	SDL_SetWindowTitle(windowId, title.c_str());
#endif

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