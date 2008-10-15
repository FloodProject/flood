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

	// Open the device
	void open();

	// Close the device
	void close();

	// Clear buffers
	void clear() = 0;

	Window& getWindow();

private:

	SDLWindow *window;

	void resetViewport();
			
};

} } // end namespaces