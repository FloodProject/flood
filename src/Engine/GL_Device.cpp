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

#ifdef VAPOR_WINDOWING_SDL
	#include <vapor/render/sdl/SDL_Window.h>
#elif
	#error "OpenGL renderer needs a windowing implementation"
#endif

namespace vapor {
	namespace render {
		namespace opengl {

GLDevice::GLDevice(WindowSettings& windowSettings)
{
	info("render::opengl", "Creating OpenGL rendering device");

	open(windowSettings);

	checkExtensions();

	_adapter = new GLAdapter();

	//_bindedVB = 0;
	//_bindedIB = 0;
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

//VertexBuffer *GLDevice::createVertexBuffer()
//{
//	GLVertexBuffer Buffer;
//	_vertexBuffers.push_back(Buffer);
//	return &_vertexBuffers.back();
//};

//void GLDevice::bindVertexBuffer(VertexBuffer *Buffer)
//{
	//if(Buffer) //If valid
	//{
	//	//Search for the real buffer (user may mistakenly use a non-GL buffer)
	//	GLVertexBuffer *RealBuffer = 0;
	//	for(std::list<GDVertexBuffer>::iterator it = _vertexBuffers.begin(); 
	//		it != _vertexBuffers.end(); it++)
	//	{
	//		if(it._Ptr == Buffer)
	//		{
	//			RealBuffer = it._Ptr;
	//			break;
	//		};
	//	};

	//	if(RealBuffer == 0) //if we didnt find it, ignore this operation
	//	{
	//		info("render::opengl::bindvertexbuffer", 
	//			"Invalid buffer submitted for binding");
	//		return;
	//	};

	//	//if the previous buffer was a VBO, we have to unbind it.
	//	if(_bindedVB && GLEW_ARB_vertex_buffer_object && glIsBufferARB(_bindedVB->ID))
	//	{
	//		glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	//	};

	//	//Bind the buffer
	//	if(GLEW_ARB_vertex_buffer_object && glIsBufferARB(RealBuffer->_ID))
	//	{
	//		glBindBufferARB(GL_ARRAY_BUFFER_ARB, RealBuffer->_ID);
	//	};

	//	//reference our buffer
	//	_bindedVB = RealBuffer;
	//}
	//else //If we don't 
	//{
	//	//Unbind buffer (doesnt matter whether the last binded VB was a VBO or not)
	//	if(GLEW_ARB_vertex_buffer_object)
	//	{
	//		glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	//	};

	//	//reference our buffer
	//	_bindedVB = 0;
	//}
//}

//void GLDevice::bindIndexBuffer(IndexBuffer *Buffer)
//{
//}

//void GLDevice::draw(unsigned long Mode, unsigned long First, unsigned long Count)
//{
//	if(_bindedVB == 0)
//	{
//		info("render::opengl::draw", "Invalid binded VB");
//		return;
//	}
//
//	if(_bindedVB->_elements.size() == 0)
//	{
//		info("render::opengl::draw", "Binded VB has no Vertex Elements");
//		return;
//	};
//
//	//Convert vaporFlag to glFlag
//	switch(Mode)
//	{
//	case PrimitiveType::TRIANGLES:
//		Mode = GL_TRIANGLES;
//		break;
//	case PrimitiveType::LINES:
//		Mode = GL_LINES;
//		break;
//	};
//}

} } } // end namespaces

#endif