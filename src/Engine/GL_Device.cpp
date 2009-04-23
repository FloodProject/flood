/************************************************************************
*
* vaporEngine by triton © (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/CompileOptions.h"

#ifdef VAPOR_RENDERER_OPENGL

#include "vapor/render/opengl/GL_Device.h"
#include "vapor/render/opengl/GL_Adapter.h"
#include "vapor/render/opengl/GL_BufferManager.h"

#ifdef VAPOR_WINDOWING_SDL
	#include <vapor/render/sdl/SDL_Window.h>
#elif
	#error "OpenGL renderer needs a windowing implementation"
#endif

namespace vapor {
	namespace render {
		namespace opengl {

GLDevice::GLDevice(Settings& settings)
{
	info("render::opengl", "Creating OpenGL rendering device");

	open(settings);

	checkExtensions();

	_adapter = new GLAdapter();
	_bufferManager = new GLBufferManager();
}

void GLDevice::close()
{
	info("render::opengl", "Closing OpenGL rendering device");

	//for(std::list<GLVertexBuffer>::iterator it = _vertexBuffers.begin(); 
	//	it != _vertexBuffers.end(); it++)
	//{
	//	// erase each buffer because we dont want to erase them after OpenGL is terminated (VBOs)
	//	_vertexBuffers.erase(it);
	//}
}

GLDevice::~GLDevice()
{
}

void GLDevice::checkExtensions()
{
	// init GLEW (OpenGL Extension Wrangler)
	GLenum err = glewInit();
	
	if(err == GLEW_OK)
		info("render::opengl", "Using GLEW version %s", glewGetString(GLEW_VERSION));
	else
		error("render::opengl", "Failed to initialize GLEW: %s", glewGetErrorString(err));
}

void GLDevice::open(Settings &settings)
{
	_window = new SDLWindow(settings);
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
	Settings &s = _window->getSettings();

	glViewport(0, 0, s.width(), s.height());

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat) s.width() / (GLfloat) s.height(), 1.0f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

} } } // end namespaces

#endif