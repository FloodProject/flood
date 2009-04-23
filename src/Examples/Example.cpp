/************************************************************************
*
* vaporEngine by triton © (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "Example.h"

#include "vapor/render/BufferManager.h"

using namespace vapor::examples;

void Example::onInit()
{
	warn("example::onInit", "Example warning message!");
	info("example::onInit", "Example info message!");
	error("example::onInit", "Example error message!");
}

void Example::onRender() 
{
	// clear the render device
	Device& device = getDevice();
	
	device.clearTarget();

	BufferManager& buf = device.getBufferManager();
	VertexBuffer* vb = buf.createVertexBuffer();
}

void Example::onSetupResources() 
{

}