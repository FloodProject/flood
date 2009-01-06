/************************************************************************
*
* vaporEngine by triton (2008)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/render/opengl/GL_Device.h"
#include "vapor/render/opengl/GL_Adapter.h"

namespace vapor {
	namespace render {
		namespace opengl {

GLDevice::GLDevice(WindowSettings& windowSettings)
{
	info("render::opengl", "Creating OpenGL rendering device");

	open(windowSettings);

	checkExtensions();

	_adapter = new GLAdapter();
}

GLDevice::~GLDevice()
{

}

void GLDevice::checkExtensions()
{
	// init GLEW (OpenGL Extension Wrangler)
	GLenum err = glewInit();
	
	if(err == GLEW_OK)
		info("render::opengl", "Using GLEW %s", glewGetString(GLEW_VERSION));
	else
		error("render::opengl", "Failed to initialize GLEW: %s", glewGetErrorString(err));
}

void GLDevice::open(WindowSettings &wS)
{
	_window = new SDLWindow(&wS);
	
	setRenderTarget(_window);

	resetViewport();
}

void GLDevice::clearTarget()
{
	glClearColor(1.0f, 1.0f, 1.0f, 0.5f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void GLDevice::resetViewport()
{
	WindowSettings &wS = _window->getWindowSettings();

	glViewport(0, 0, wS.width, wS.height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat) wS.width / (GLfloat) wS.height, 1.0f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

} } } // end namespaces
