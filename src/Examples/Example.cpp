/************************************************************************
*
* vaporEngine by triton © (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "Example.h"

#include <vapor/resources/Image.h>
#include <vapor/resources/MS3D.h>

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
	
	vfs::File file("media/triton.png");
	Image* img =  static_cast<Image*>(rm->createResource(file));

	info("onSetupResources", "Image has pixel format '%s' and size %dx%d", 
		PixelFormat::getString(img->getPixelFormat()).c_str(), 
		img->getWidth(), img->getHeight());

	file = vfs::File("media/cubo.ms3d");
	MS3D* mesh =  static_cast<MS3D*>(rm->createResource(file));
}

//-----------------------------------//

void Example::onRender() 
{
	Device* device = getDevice();

	// clear the render device with white
	device->setClearColor(Colors::White);
	device->clearTarget();

	BufferManager* buf = device->getBufferManager();
	VertexBuffer* vb = buf->createVertexBuffer();
}

//-----------------------------------//

void Example::onUpdate() 
{

}

//-----------------------------------//

#define VAPOR_EXAMPLE_NAME Example
#include <vapor/Main.h>