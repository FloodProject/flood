/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "Engine.h"

#include "TaskManager.h"
#include "vfs/FileSystem.h"
#include "render/Device.h"
#include "input/InputManager.h"
#include "audio/Device.h"
#include "script/ScriptManager.h"
#include "scene/Scene.h"
#include "paging/PageManager.h"
#include "resources/ResourceManager.h"
#include "physics/Physics.h"
#include "ResourceLoaders.h"

namespace vapor {

//-----------------------------------//

Engine::Engine()
	: log(nullptr)
	, stream(nullptr)
	, fileSystem(nullptr)
	, resourceManager(nullptr)
	, renderDevice(nullptr)
	, audioDevice(nullptr)
	, physicsManager(nullptr)
	, scriptManager(nullptr)
{ }

//-----------------------------------//

void Engine::create(const std::string& app, const char** argv)
{
	this->app = app;
	this->argv = argv;
}

//-----------------------------------//

Engine::~Engine()
{
	scene.reset();

	for( uint i = 0; i < subsystems.size(); i++ )
	{
		Subsystem* system = subsystems[i];
		delete system;
	}
	
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
	// Sets up the global logger.
	setupLogger();

	Log::info( "Starting vaporEngine version '%s'", VAPOR_ENGINE_VERSION );

	// Creates the file system.
	fileSystem = new FileSystem( app, argv ? argv[0] : nullptr );

	// Creates the task system.
	taskManager = new TaskManager();

	// Creates the resource manager.
	FileWatcher* fw = fileSystem->getFileWatcher();
	resourceManager = new ResourceManager( fw, taskManager );

	// Registers default resource loaders.
	setupResourceLoaders();

	// Creates the rendering and audio devices.
	setupDevices( createWindow );

	// Creates the initial scene.
	scene.reset( new Scene() );

#ifdef VAPOR_SCRIPTING_LUA
	// Creates the scripting manager.
	scriptManager = new ScriptManager();
#endif

#ifdef VAPOR_PHYSICS_BULLET
	// Creates the physics manager.
	physicsManager = new PhysicsManager();
#endif
}

//-----------------------------------//

void Engine::setupLogger()
{
	log = new Logger();

	stream = new FileStream("Log.html", StreamMode::Write);
	log->add( new LogSinkHTML(*stream) );
}

//-----------------------------------//

void Engine::setupDevices( bool createWindow )
{
	renderDevice = new RenderDevice( resourceManager );

	if( createWindow )
	{
		// Creates a new render window.
		renderDevice->createWindow();

		// Initializes the render device with new window context.
		renderDevice->init();
		
		// Creates the input devices.
		setupInput();
	}

#ifdef VAPOR_AUDIO_OPENAL
	// Creates the audio device.
	audioDevice = new audio::Device();
#endif
}

//-----------------------------------//

void Engine::setupInput()
{
	Window* window = renderDevice->getWindow();
	
	if(!window)
		return;

	InputManager* input = window->getInputManager();
	
	if(!input)
		return;

	input->createDefaultDevices(); 
}

//-----------------------------------//

void Engine::setupResourceLoaders()
{
	std::vector<ResourceLoader*> loaders;

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

	#ifdef VAPOR_IMAGE_DEVIL
		loaders.push_back( new IL_Image_Loader() );
	#endif
	
	#ifdef VAPOR_FONT_BITMAP
		loaders.push_back( new Font_Loader() );
	#endif

	for( uint i = 0; i < loaders.size(); i++ )
	{
		ResourceLoader* loader = loaders[i];
		resourceManager->registerLoader( loader );
	}
}

//-----------------------------------//

void Engine::update( double delta )
{
	for( uint i = 0; i < subsystems.size(); i++ )
	{
		Subsystem* system = subsystems[i];
		system->update( delta );
	}

	fileSystem->update( delta );
	resourceManager->update( delta );
	scene->update( delta );

#ifdef VAPOR_SCRIPTING_LUA
	scriptManager->update( delta );
#endif

#ifdef VAPOR_PHYSICS_BULLET
	physicsManager->update( delta );
#endif
}

//-----------------------------------//

InputManager* Engine::getInputManager() const
{
	Window* window = renderDevice->getWindow();

	if( !window )
		return nullptr;

	return window->getInputManager();
}

//-----------------------------------//

} // end namespace