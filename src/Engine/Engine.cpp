/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/Engine.h"

#ifdef VAPOR_IMAGE_PICOPNG
	#include "vapor/resources/PNG_Loader.h"
#endif

#ifdef VAPOR_MESH_MILKSHAPE3D
	#include "vapor/resources/MS3D_Loader.h"
#endif

#ifdef VAPOR_AUDIO_OGG
	#include "vapor/resources/OGG_Loader.h"
#endif

#ifdef VAPOR_AUDIO_OPENAL
	#include "vapor/audio/Device.h"
#endif

using namespace vapor::audio;
using namespace vapor::scene;
using namespace vapor::resources;
using namespace vapor::render;
using namespace vapor::log;
using namespace vapor::vfs;

namespace vapor {

//-----------------------------------//

Engine::Engine(std::string app, bool autoInit)
	: app(app)
{
	if(autoInit)
	{
		init();
	}
}

//-----------------------------------//

Engine::~Engine()
{
	delete sceneNode;

#ifdef VAPOR_AUDIO_OPENAL
	delete audioDevice;
#endif

	delete renderDevice;
	delete resourceManager;
	delete vfs;
	delete Log::getLogger();
}

//-----------------------------------//

void Engine::init()
{
	// setup the global logger
	setupLogger();

	info( "engine", "Starting vaporEngine version '%s'", VAPOR_ENGINE_VERSION );

	// create the virtual filesystem
	vfs = new VFS(app);

	// create the resource manager
	resourceManager = new ResourceManager();

	// register default codecs
	setupResourceLoaders();

	// create a rendering and audio device
	setupDevices();

	// create the root scene node
	sceneNode = new scene::Scene();
}

//-----------------------------------//

void Engine::setupLogger()
{
	// get a suitabfle log filename
	std::string file = app + ".html";
	
	// create and set a new logger
	log = new Log(app, file);
	Log::setLogger(log);
}

//-----------------------------------//

void Engine::setupDevices()
{
	// create render device
	renderDevice = new render::Device();

	// create a window and set the title
	renderDevice->createWindow(app);

	// init the render device now that it has a context
	getRenderDevice()->init();

#ifdef VAPOR_AUDIO_OPENAL

	// create the audio device
	audioDevice = new audio::Device();

#endif
}

//-----------------------------------//

void Engine::setupResourceLoaders()
{
	if(!resourceManager) return;

	std::vector<ResourceLoader*> loaders;
	ResourceLoader* loader = nullptr;

	// register default compiled codecs
	#ifdef VAPOR_IMAGE_PICOPNG
		loader = new PNG_Pico_Loader();
		loaders.push_back(loader);
	#endif

	#ifdef VAPOR_MESH_MILKSHAPE3D
		loader = new MS3D_Loader();
		loaders.push_back(loader);
	#endif

	#ifdef VAPOR_AUDIO_OGG
		loader = new OGG_Loader();
		loaders.push_back(loader);
	#endif

	std::vector<ResourceLoader*>::iterator it;
	for(it = loaders.begin(); it != loaders.end(); it++)
	{
		resourceManager->registerLoader(*it);
	}
}

//-----------------------------------//

} // end namespace