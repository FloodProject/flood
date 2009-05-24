/************************************************************************
*
* vaporEngine by triton © (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "Example.h"

//-----------------------------------//

void Example::onInit()
{
	warn("example::onInit", "Example warning message!");
	info("example::onInit", "Example info message!");
	error("example::onInit", "Example error message!");
}

//-----------------------------------//

void Example::onSetupResources() 
{
	ResourceManager* rm = getResourceManager();
	
	//vapor::vfs::File& file("data/triton.png");
	//Resource* img = rm->addResource(file);
}

//-----------------------------------//

void Example::onRender() 
{
	// clear the render device
	Device* device = getDevice();

	device->clearTarget();

	BufferManager& buf = device->getBufferManager();
	VertexBuffer* vb = buf.createVertexBuffer();
}

//-----------------------------------//

#define VAPOR_EXAMPLE_NAME Example
#include <vapor/Main.h>