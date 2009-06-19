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
	#include "vapor/audio/al/AL_Device.h"
#endif

namespace vapor {

//-----------------------------------//

Engine::Engine()
{
	// create the resource manager
	resourceManager = new ResourceManager();
}

//-----------------------------------//

Engine::~Engine()
{
	// delete stuff
	delete device;
	delete resourceManager;
	delete log::Log::getLogger();
}

//-----------------------------------//

void Engine::setupLogger(string title, string file)
{
	log = new Log(title, file);
	Log::setLogger(log);
}

//-----------------------------------//

void Engine::setupDevices()
{
	// create render device
	device = Device::createDevice();

	// TODO: hardcoded title...
	// set the window title
	device->getWindow()->setTitle("vaporEngine Example");

	audioDevice = new audio::AL();

	// create the root scene node
	sceneNode = new scene::Scene();
}

//-----------------------------------//

void Engine::setupResourceLoaders()
{
	ResourceManager* rm = getResourceManager();
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
		rm->registerResourceLoader(*it);
	}
}

//-----------------------------------//

} // end namespace