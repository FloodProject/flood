/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//// 
//// fuckEngine by triton
////
//// License: fuckGNU License
////
//// Thanks to:
////	Zero (love ya bitch)
////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#include "vapor/SDL_Window.h"

namespace vapor {
	namespace render {


SDLWindow::SDLWindow(const string& title, shared_ptr<WindowSettings> windowSettings)
	:	Window(title, windowSettings)
{
	this->windowSettings = windowSettings;

	if (!init() || !open())
		//LOGME
		exit(1);

	setTitle(title);
}

SDLWindow::~SDLWindow()
{
	// shutdown SDL
	SDL_Quit();
}

bool SDLWindow::init(void)
{
	display = NULL;

	// initialize video sub-system
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		return false;

	// get some information about the video
	vidinfo = (SDL_VideoInfo*) SDL_GetVideoInfo();

	return true;
}

bool SDLWindow::open()
{
	Uint32 flags = SDL_OPENGL;

	// check if we want fullscreen
	if (windowSettings->fullscreen)
		flags |= SDL_FULLSCREEN;

	// check if we have hardware acceleration
	if(vidinfo->hw_available)
		flags |= SDL_HWSURFACE;

	//SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
	//SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
	//SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
	//SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 5 );
	//SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

	// set the video mode
	display = SDL_SetVideoMode(windowSettings->width,
		windowSettings->height, windowSettings->bpp, flags);

	if (!display)
		// we are in deep shit
		return false;

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

	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{

		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE)
				return false;
			else
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
}

void SDLWindow::setCursor(bool state) const
{
	SDL_ShowCursor(state);
}

} } // end namespaces