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

using namespace vapor::resources;
using namespace vapor::render;
using namespace vapor::log;

namespace vapor {

//-----------------------------------//

Engine::Engine(bool autoInit)
{
	if(!autoInit) return;
	
	setupResourceManager();
	setupLogger("vaporEngine", "vaporEngine.html");
	setupResourceLoaders();
	setupDevices();
	setupWindow("vaporEngine");
}

//-----------------------------------//

Engine::~Engine()
{
	delete resourceManager;
	delete sceneNode;
	delete Log::getLogger();
	delete audioDevice;
	delete renderDevice;
}

//-----------------------------------//

void Engine::setupLogger(string title, string file)
{
	log = new Log(title, file);
	Log::setLogger(log);
}

//-----------------------------------//

void Engine::setupResourceManager()
{
	// create the resource manager
	resourceManager = new ResourceManager();
}

//-----------------------------------//

void Engine::setupDevices()
{
	// create render device
	renderDevice = new render::Device();

	// create the audio device
	audioDevice = new audio::Device();

	// create the root scene node
	sceneNode = new scene::Scene();
}

//-----------------------------------//

void Engine::setupWindow(string title)
{
	if(!renderDevice) return;

	// create a window and set the title
	renderDevice->createWindow();
	renderDevice->getWindow()->setTitle(title);
}

//-----------------------------------//

void Engine::setupResourceLoaders()
{
	if(!resourceManager) return;

	vector<ResourceLoader*> loaders;
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

	vector<ResourceLoader*>::iterator it;
	for(it = loaders.begin(); it != loaders.end(); it++)
	{
		resourceManager->registerResourceLoader(*it);
	}
}

//-----------------------------------//

} // end namespace