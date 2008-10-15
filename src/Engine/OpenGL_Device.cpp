#include "vapor/OpenGL_Device.h"

namespace vapor {
	namespace render {

OpenGLDevice::OpenGLDevice(shared_ptr<WindowSettings> wS)
{
	window = new SDLWindow("",wS);

	open();
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

void OpenGLDevice::clear()
{
	glClearColor(1.0f, 1.0f, 0.5f, 0.5f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLDevice::resetViewport()
{
	WindowSettings &wS = window->getWindowSettings();

	glViewport(0, 0, wS.width, wS.height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat) wS.width / (GLfloat) wS.height, 1.0f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

} } // end namespaces