#pragma once

#include "vapor/RenderDevice.h"
#include "vapor/SDL_Window.h"

namespace vapor {
	namespace render {

class OpenGLDevice : public RenderDevice
{

public:

	OpenGLDevice();
	virtual ~OpenGLDevice();

	Window& getWindow();

private:

	SDLWindow *window;
			
};

} } // end namespaces