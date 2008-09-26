#include "Engine.h"

using namespace fuck::resources;
using namespace fuck::renderer;

const int WINDOW_WIDTH = 320;
const int WINDOW_HEIGHT = 240;

int main(int argc, char *argv[])
{
	// create the logger
	Log logger(ENGINE_NAME, "log.html");
	
	logger.info("Rendering", "Creating %dx%d window", WINDOW_WIDTH, WINDOW_HEIGHT);

	// create display window
	Window &window = Window::createWindow("fuckEngine Example",
		WINDOW_WIDTH, WINDOW_HEIGHT, false);
	
	// load png image
	PNG png("media/triton.png");

	SDL_Surface* s = convertPNGToSurface(png);
	SDL_Surface* f = Font::getSurface(ENGINE_NAME);
	SDL_Surface* fps;

	do 
	{
		window.clear();

		fps = Font::getSurface(window.getFPS());
		
		window.blit(fps,WINDOW_WIDTH-fps->w-5,fps->h-5);
		window.blit(s,100,100);
		window.blit(f,5,5);

		window.update();

		SDL_FreeSurface(fps);

	} while ( window.events() );

	SDL_FreeSurface(s);
	SDL_FreeSurface(f);

	return 0;
}