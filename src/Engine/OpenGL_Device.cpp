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

void OpenGLDevice::open()
{
	resetViewport();
}

void OpenGLDevice::resetViewport()
{
	WindowSettings &wS = window->getWindowSettings();

	glViewport(0, 0, wS.width, wS.height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)wS.width / (GLfloat) wS.height, 1.0f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

} } // end namespaces