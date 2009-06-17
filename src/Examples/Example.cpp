/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "Example.h"

#include <vapor/resources/Image.h>
#include <vapor/resources/MS3D.h>
#include <vapor/resources/Sound.h>

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

	file = vfs::File("media/stereo.ogg");
	Sound* sound =  static_cast<Sound*>(rm->createResource(file));

	getAudioDevice()->play2D(sound);
}

//-----------------------------------//

void Example::onRender() 
{
	Device* device = getDevice();

	// clear the render device with white
	device->setClearColor(Colors::White);
	device->clearTarget();

	// create a vertex buffer
	BufferManager* bm = device->getBufferManager();
	
	// declare the vertex elements
	VertexElement elms[] = {
		{0, VertexAttribute::Position, VertexDataType::float3}
	};

	// construct a vertex declaration from the elements
	VertexDeclaration decl(elms, elms + (sizeof(elms) / sizeof(elms[0])));

	// create a static write-only vertex buffer for 10 elements
	shared_ptr<VertexBuffer> vb = bm->createVertexBuffer(
		10, decl, BufferUsage::Write, BufferType::Static);
}

//-----------------------------------//

void Example::onUpdate() 
{

}

//-----------------------------------//

#define VAPOR_EXAMPLE_NAME Example
#include <vapor/Main.h>