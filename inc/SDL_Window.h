#pragma once

#include <SDL.h>
#include <SDL_opengl.h>

#include "Window.h"

namespace vapor {
	namespace renderer {

class SDLWindow : public Window
{
public:

	SDLWindow(const std::string &,const int,const int,const bool);
	virtual ~SDLWindow();

	bool open (const string& title, const int width, const int height, const bool fullscreen);

	bool clear ();

	void update ();

	void blit(SDL_Surface *surface, int x, int y);

	const char* getFPS();

	bool events();

	void setTitle(const string& title) const;

	void setCursor(bool state) const;


private:

	int FPS, pastFPS, past;

	SDL_VideoInfo	*vidinfo;			// video information

	SDL_Surface		*display;			// the actual display surface

	// Initializes the Display system
	bool init();
};

} } // end namespaces