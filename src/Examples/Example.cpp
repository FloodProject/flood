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

#include <vapor/scene/Sound.h>
#include <vapor/scene/Listener.h>

using namespace vapor;
using namespace vapor::render;
using namespace vapor::audio;
using namespace vapor::scene;
using namespace vapor::math;
using namespace vapor::resources;

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

	file = vfs::File("media/cubo.ms3d");
	MS3D* mesh =  static_cast<MS3D*>(rm->createResource(file));

	file = vfs::File("media/stereo.ogg");
	resources::Sound* sound =  static_cast<resources::Sound*>(rm->createResource(file));

	//getAudioDevice()->play2D(sound);
}

//-----------------------------------//

void Example::onSetupScene() 
{
	Scene* scene = getSceneManager();

	shared_ptr<Listener> ls(new Listener());
	ls->translate(math::Vector3(1.0f, 0.0f, 0.0f));
	ls->makeCurrent();

	scene->add(ls);
	
	//scene.add(new scene::
}

//-----------------------------------//

void Example::onRender() 
{
	Device* device = getRenderDevice();

	// clear the render device with white
	device->setClearColor(Colors::White);
	device->clearTarget();

	//// create a vertex buffer
	//BufferManager* bm = device->getBufferManager();
	//
	//// declare the vertex elements
	//VertexElement elms[] = {
	//	{0, VertexAttribute::Position, VertexDataType::float3}
	//};

	//// construct a vertex declaration from the elements
	//VertexDeclaration decl(elms, elms + (sizeof(elms) / sizeof(elms[0])));

	//// create a static write-only vertex buffer for 10 elements
	//shared_ptr<VertexBuffer> vb = bm->createVertexBuffer(
	//	10, decl, BufferUsage::Write, BufferType::Static);
}

//-----------------------------------//

void Example::onUpdate() 
{

}

//-----------------------------------//

#define VAPOR_EXAMPLE_NAME Example
#include <vapor/Main.h>