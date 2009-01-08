/************************************************************************
*
* vaporEngine by triton (2008)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#define GLEW_STATIC 
#include <GL/glew.h>

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
