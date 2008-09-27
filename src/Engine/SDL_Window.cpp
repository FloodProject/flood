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

#include "vapor/Engine.h"
#include "vapor/SDL_Window.h"

void setupViewport(int x, int y)
{ 
	glViewport(0, 0, x, y);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)x / (GLfloat) y, 1.0f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


SDLWindow::SDLWindow(const string& title, const int width, const int height, const bool fullscreen)
:	Window(width, height), FPS(0), pastFPS(0), past(0)
{
	if (!init())
		// we are doomed
		exit(1);

	open(title, width, height, fullscreen);

	setupViewport(width, height);
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

bool SDLWindow::open(const string& title, const int width, const int height, const bool fullscreen)
{
	Uint32 flags = SDL_OPENGL;

	// check if we want fullscreen
	if (fullscreen)
		flags |= SDL_FULLSCREEN;

	// check if we have hardware acceleration
	if(vidinfo->hw_available)
		flags |= SDL_HWSURFACE;

	SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
	SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
	SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
	SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 5 );
	SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

	// set the video mode
	display = SDL_SetVideoMode(width, height, 32, flags);
	
	setTitle(title);

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

void SDLWindow::blit(SDL_Surface *surface, int x, int y)
{
	SDL_Rect disp;
	disp.x = x; disp.y = y; disp.w = surface->w; disp.h = surface->h;

	SDL_BlitSurface (surface, NULL, display, &disp);
}

const char* SDLWindow::getFPS()
{
	static char buffer[20] = {0};
    int currentTime = SDL_GetTicks();
   
    if ( currentTime - past >= 16 )
    {
      past = SDL_GetTicks();   // this should be done before redrawing screen     

      //SDL_Flip( screen );
      FPS++;
    }
   
    if ( currentTime - pastFPS >= 1000 )
    {
	  #pragma warning(disable : 4996)
      sprintf( buffer, "%d FPS", FPS );
     
      FPS = 0;
      pastFPS = currentTime;
    }

	return buffer;
}

bool SDLWindow::clear ()
{
	return SDL_FillRect(display, NULL, 0) == 0;
}

bool SDLWindow::events()
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