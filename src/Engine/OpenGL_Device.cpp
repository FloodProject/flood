#include "vapor/OpenGL_Device.h"

namespace vapor {
	namespace render {

OpenGLDevice::OpenGLDevice()
{
	shared_ptr<WindowSettings> wS(new WindowSettings(320, 240));
	window = new SDLWindow("Testing!", wS);
}

OpenGLDevice::~OpenGLDevice()
{

}

Window& OpenGLDevice::getWindow()
{
	return *window;
}

} } // end namespaces