/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/CompileOptions.h"

#ifdef VAPOR_RENDERER_OPENGL

#include "vapor/render/Device.h"
#include "vapor/render/Adapter.h"
#include "vapor/render/BufferManager.h"

#include "vapor/render/GL.h"

#if defined(VAPOR_WINDOWING_SDL)
	#include <vapor/render/SDL_Window.h>
#elif defined(VAPOR_WINDOWING_WIN32)
	#include <vapor/render/Win32_Window.h>
#elif defined(VAPOR_WINDOWING_SFML)
	#include <vapor/render/SFML_Window.h>
#else
	#error "OpenGL renderer needs a windowing implementation"
#endif

namespace vapor {
	namespace render {

//-----------------------------------//

Device::Device()
	: clearColor(math::Colors::White), adapter(nullptr),
	window(nullptr), bufferManager(nullptr)
{

}

//-----------------------------------//

void Device::init()
{
	info( "render::gl", "Creating OpenGL rendering device" );

	if( !activeTarget || !window ) 
	{
		warn( "render::gl", "No current OpenGL context found, stuff may fail" );
	}

	checkExtensions();

	adapter = new Adapter();
	bufferManager = new BufferManager();
}

//-----------------------------------//

Device::~Device()
{
	info("render::opengl", "Closing OpenGL rendering device");

	//for(std::list<GLVertexBuffer>::iterator it = _vertexBuffers.begin(); 
	//	it != _vertexBuffers.end(); it++)
	//{
	//	// erase each buffer because we dont want to erase them after OpenGL is terminated (VBOs)
	//	_vertexBuffers.erase(it);
	//}

	delete adapter;
	delete window;
	delete bufferManager;
}

//-----------------------------------//

Window* Device::getWindow() const
{
	return window;
}

//-----------------------------------//

Adapter* Device::getAdapter() const
{
	return adapter;
}

//-----------------------------------//

void Device::setClearColor(math::Color c)
{
	clearColor = c;
}

//-----------------------------------//

void Device::updateTarget()
{
	activeTarget->update();
}

//-----------------------------------//

void Device::setRenderTarget(RenderTarget* renderTarget)
{
	activeTarget = renderTarget;
}

//-----------------------------------//

BufferManager* Device::getBufferManager() const
{
	return bufferManager;
}

//-----------------------------------//

void Device::checkExtensions()
{
	// init GLEW (OpenGL Extension Wrangler)
	GLenum err = glewInit();
	
	if(err == GLEW_OK)
		info("render::opengl", "Using GLEW version %s", glewGetString(GLEW_VERSION));
	else
		error("render::opengl", "Failed to initialize GLEW: %s", glewGetErrorString(err));
}

//-----------------------------------//

Window& Device::createWindow(const std::string title, Settings settings)
{
#if defined(VAPOR_WINDOWING_SDL)
	window = new SDLWindow(settings);
#elif defined(VAPOR_WINDOWING_WIN32)
	window = new Win32Window(settings);
#elif defined(VAPOR_WINDOWING_SFML)
	window = new SFMLWindow(settings);
#else
	#error "Could not find a window implementation"
#endif

	window->setTitle(title);

	setRenderTarget(window);
	resetViewport();

	return *window;
}

//-----------------------------------//

void Device::clearTarget()
{
	glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
	glClear(GL_COLOR_BUFFER_BIT);
}

//-----------------------------------//

void Device::resetViewport()
{
	Settings &s = window->getSettings();

	glViewport(0, 0, s.getWidth(), s.getHeight());

	// TODO: Take this shit out of here... this is Camera stuff.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat) s.getWidth() / (GLfloat) s.getHeight(), 1.0f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//-----------------------------------//

} } // end namespaces

#endif