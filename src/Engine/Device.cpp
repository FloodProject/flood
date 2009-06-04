/************************************************************************
*
* vaporEngine by triton © (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/CompileOptions.h"

#ifdef VAPOR_RENDERER_OPENGL
	#include "vapor/render/gl/GL_Device.h"
#endif

#include "vapor/render/Device.h"

namespace vapor {
	namespace render {

//-----------------------------------//

Device* Device::createDevice(Settings settings)
{
	#ifdef VAPOR_RENDERER_OPENGL
		return new opengl::GLDevice(settings);
	#else
		#error "A renderer is needed to compile the engine"
	#endif
}

//-----------------------------------//

Device::Device()
	: clearColor(Colors::White)
{

}

//-----------------------------------//

Device::~Device()
{
	delete adapter;
	delete window;
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

void Device::setClearColor(Color c)
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

} } // end namespaces
