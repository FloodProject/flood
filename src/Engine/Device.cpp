/************************************************************************
*
* vaporEngine by triton (2008)
*
*	<http://www.portugal-a-programar.org>
*
* DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
* Version 2, December 2004
* 
* DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
* TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
* 
* 0. You just DO WHAT THE FUCK YOU WANT TO.
*
************************************************************************/

#include "vapor/render/opengl/GL_Device.h"

namespace vapor {
	namespace render {

Device::Device()
{

}

Device::~Device()
{
	delete _adapter;
	delete _window;
}

Window& Device::getWindow()
{
	return *_window;
}

Adapter& Device::getAdapter()
{
	return *_adapter;
}

void Device::updateTarget()
{
	_activeTarget->update();
}

void Device::setRenderTarget(RenderTarget* renderTarget)
{
	_activeTarget = renderTarget;
}

BufferManager* Device::getBufferManager()
{
	return _bufferManager;
}

Device* Device::createDevice(WindowSettings wS)
{
	return new opengl::GLDevice(wS);
}

} } // end namespaces
