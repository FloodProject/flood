/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

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

#ifdef VAPOR_SHADER_GLSL
	#include "vapor/resources/GLSL_Loader.h"
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
using namespace vapor::input;

namespace vapor {

//-----------------------------------//

Engine::Engine(std::string app, const char** argv, bool autoInit)
	: app(app), argv(argv)
{
	if(autoInit)
	{
		init();
	}
}

//-----------------------------------//

Engine::~Engine()
{
	//delete sceneNode;

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
	vfs = new VFS(app, argv ? argv[0] : nullptr);

	// create the resource manager
	resourceManager = new ResourceManager();

	// register default codecs
	setupResourceLoaders();

	// create a rendering and audio device
	setupDevices();

	// create the root scene node
	sceneNode.reset( new scene::Scene() );
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
	Window& window = renderDevice->createWindow();

	// init the render device now that it has a context
	renderDevice->init();

	InputManager& im = window.getInputManager();

	// let's register some input devices
	im.addDevice( new input::Keyboard() );
	im.addDevice( new input::Mouse() );

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

	// register default compiled codecs
	#ifdef VAPOR_IMAGE_PICOPNG
		loaders.push_back( new PNG_Pico_Loader() );
	#endif

	#ifdef VAPOR_MESH_MILKSHAPE3D
		loaders.push_back( new MS3D_Loader() );
	#endif

	#ifdef VAPOR_AUDIO_OGG
		loaders.push_back( new OGG_Loader() );
	#endif

	#ifdef VAPOR_SHADER_GLSL
		loaders.push_back( new GLSL_Loader() );
	#endif

	foreach( ResourceLoader* loader, loaders )
	{
		resourceManager->registerLoader( loader );
	}
}

//-----------------------------------//

} // end namespace