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

using std::tr1::static_pointer_cast;

using namespace vapor;
using namespace vapor::vfs;
using namespace vapor::math;
using namespace vapor::scene;
using namespace vapor::audio;
using namespace vapor::render;
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
	
	File file("media/triton.png");
	shared_ptr<Resource> img = rm->createResource(file);

	file = File("media/stereo.ogg");
	shared_ptr<Resource> sound = rm->createResource(file);
}

//-----------------------------------//

void Example::onSetupScene() 
{
	ResourceManager* rm = getResourceManager();
	Scene* scene = getSceneManager();
	audio::Device* audioDev = getAudioDevice();

	shared_ptr<Group> grp(new Group());

	shared_ptr<Listener> ls(new Listener(audioDev));
	ls->translate(math::Vector3(1.0f, 0.0f, 0.0f));
	ls->makeCurrent();
	ls->setVolume(1.0f);
	grp->add(ls);

	File file("media/stereo.ogg");
	shared_ptr<resources::Sound> res = 
		static_pointer_cast<resources::Sound>(rm->getResource(file));
	shared_ptr<scene::Sound> snd(new scene::Sound(ls, res));
	grp->add(snd);

	scene->add(grp);

	snd->play(5);

	string example = scene->save();
	puts(example.c_str());
}

//-----------------------------------//

void Example::onRender()
{
	render::Device* device = getRenderDevice();

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
	//shared_ptr<VertexBuffer> vb(new VertexBuffer(
	//	10, decl, BufferUsage::Write, BufferType::Static));
}

//-----------------------------------//

void Example::onUpdate() 
{
	Scene* scene = getSceneManager();
}

//-----------------------------------//