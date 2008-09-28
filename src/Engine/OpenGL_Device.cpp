#include "vapor/OpenGL_Device.h"

namespace vapor {
	namespace render {

OpenGLDevice::OpenGLDevice()
{
	window = new SDLWindow();
}

OpenGLDevice::~OpenGLDevice()
{

}

Window& OpenGLDevice::getWindow()
{
	return *window;
}

} } // end namespaces