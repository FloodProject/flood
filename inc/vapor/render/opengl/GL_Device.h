/************************************************************************
*
* vaporEngine by triton © (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/CompileOptions.h"

#ifdef VAPOR_RENDERER_OPENGL

#ifdef VAPOR_WINDOWING_SDL
	#define GLEW_STATIC 
	#include <GL/glew.h>

	// needed because SDL_OpenGL conflicts with GLEW
	#define NO_SDL_GLEXT
	#include <SDL_opengl.h>
#endif

#include "vapor/render/Device.h"
#include "vapor/render/SDL_Window.h"
#include "vapor/render/VertexBuffer.h"

namespace vapor {
	namespace render {
		namespace opengl {

class GLDevice : public Device
{

public:

	GLDevice(WindowSettings& wS);

	virtual ~GLDevice();

	// Open the device
	void open(WindowSettings& wS);

	// Close the device
	void close();

	// Clear buffers
	void clearTarget();

private:

	void resetViewport();

	void checkExtensions();
};

} } } // end namespaces

#endif