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
{

}

//-----------------------------------//

Device::~Device()
{
	delete _adapter;
	delete _window;
}

//-----------------------------------//

Window& Device::getWindow()
{
	return *_window;
}

//-----------------------------------//

Adapter& Device::getAdapter()
{
	return *_adapter;
}

//-----------------------------------//

void Device::updateTarget()
{
	_activeTarget->update();
}

//-----------------------------------//

void Device::setRenderTarget(RenderTarget* renderTarget)
{
	_activeTarget = renderTarget;
}

//-----------------------------------//

BufferManager& Device::getBufferManager()
{
	return *_bufferManager;
}

//-----------------------------------//

} } // end namespaces
