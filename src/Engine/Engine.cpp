/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/Engine.h"

#include "vapor/TaskManager.h"
#include "vapor/vfs/FileSystem.h"
#include "vapor/render/Device.h"
#include "vapor/input/InputManager.h"
#include "vapor/audio/Device.h"
#include "vapor/script/ScriptManager.h"
#include "vapor/scene/Scene.h"
#include "vapor/paging/PageManager.h"
#include "vapor/resources/ResourceManager.h"
#include "ResourceLoaders.h"
#include "vapor/physics/Physics.h"

namespace vapor {

//-----------------------------------//

Engine::Engine()
	: log(nullptr)
	, fileSystem(nullptr)
	, resourceManager(nullptr)
	, renderDevice(nullptr)
	, audioDevice(nullptr)
	, physicsManager(nullptr)
	, scriptManager(nullptr)
{ }

//-----------------------------------//

void Engine::create(const std::string& _app, 
					const char** _argv, bool autoInit)
{
	app = _app;
	argv = _argv;

	if( autoInit )
		init();
}

//-----------------------------------//

Engine::~Engine()
{
	scene.reset();

	foreach( Subsystem* const sub, subsystems )
		delete sub;
	
	delete taskManager;
	//delete audioDevice;
	delete physicsManager;
	delete scriptManager;
	delete renderDevice;
	delete resourceManager;
	delete fileSystem;
	delete log;
}

//-----------------------------------//

void Engine::addSubsystem( Subsystem* const subsystem )
{
	Log::info( "Registering new engine subsystem" );
	
	subsystems.push_back( subsystem );
}

//-----------------------------------//

void Engine::init( bool createWindow )
{
	// setup the global logger
	setupLogger();

	Log::info( "Starting vaporEngine version '%s'", VAPOR_ENGINE_VERSION );

	// create the virtual filesystem
	fileSystem = new FileSystem( app, argv ? argv[0] : nullptr );

	taskManager = new TaskManager();

	// create the resource manager
	FileWatcher* fw = fileSystem->getFileWatcher();
	resourceManager = new ResourceManager( fw, taskManager );

	// create the physics manager
	physicsManager = new PhysicsManager();

	// register default codecs
	setupResourceLoaders();

	// create a rendering and audio device
	setupDevices( createWindow );

	// create the root scene node
	scene.reset( new Scene() );

#ifdef VAPOR_SCRIPTING_LUA
	// Initialize the scripting
	scriptManager = new ScriptManager();
#endif
}

//-----------------------------------//

void Engine::setupLogger()
{
	#pragma TODO(Create log files in a folder)

	// Create a new logger.
	log = new Logger(app, app + ".html");
}

//-----------------------------------//

void Engine::setupDevices( bool createWindow )
{
	// create render device
	renderDevice = new RenderDevice( resourceManager );

	if( createWindow )
	{
		// create a window and set the title
		renderDevice->createWindow();

		// init the render device now that it has a context
		renderDevice->init();
		
		// creates the default input devices
		setupInput();
	}

#ifdef VAPOR_AUDIO_OPENAL

	// create the audio device
	audioDevice = new audio::Device();

#endif
}

//-----------------------------------//

void Engine::setupInput()
{
	Window* window = renderDevice->getWindow();
	InputManager* im = window->getInputManager();
	im->createDefaultDevices(); 
}

//-----------------------------------//

void Engine::setupResourceLoaders()
{
	assert( resourceManager != nullptr );

	std::vector<ResourceLoader*> loaders;

	// register default compiled codecs
	#ifdef VAPOR_IMAGE_PICOPNG
		loaders.push_back( new PNG_Pico_Loader() );
	#endif

	#ifdef VAPOR_MESH_MILKSHAPE3D
		loaders.push_back( new Milkshape3D_Loader() );
	#endif

	#ifdef VAPOR_AUDIO_OGG
		loaders.push_back( new OGG_Loader() );
	#endif

	#ifdef VAPOR_SHADER_GLSL
		loaders.push_back( new GLSL_Loader() );
	#endif

	#ifdef VAPOR_SCRIPTING_LUA
		loaders.push_back( new Lua_Loader() );
	#endif

	#ifdef VAPOR_IMAGE_STB
		loaders.push_back( new STB_Image_Loader() );
	#endif
	
	#ifdef VAPOR_FONT_BITMAP
		loaders.push_back( new Font_Loader() );
	#endif

	foreach( ResourceLoader* const loader, loaders )
	{
		resourceManager->registerLoader( loader );
	}
}

//-----------------------------------//

void Engine::update( double delta )
{
	foreach( Subsystem* subsystem, subsystems )
		subsystem->update( delta );

	fileSystem->update( delta );
	resourceManager->update( delta );
	scene->update( delta );

#ifdef VAPOR_SCRIPTING_LUA
	scriptManager->update( delta );
#endif

	physicsManager->update( delta );
}

//-----------------------------------//

InputManager* Engine::getInputManager() const
{
	Window* window = renderDevice->getWindow();
	return window->getInputManager();
}

//-----------------------------------//

} // end namespace