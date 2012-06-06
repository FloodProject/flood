/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"
#include "Engine/Engine.h"

#include "Core/Memory.h"
#include "Resources/ResourceManager.h"
#include "Resources/ResourceLoader.h"
#include "Graphics/Texture.h"
#include "Graphics/RenderDevice.h"
#include "Graphics/Render.h"
#include "Input/InputManager.h"
#include "Audio/Device.h"
#include "Script/ScriptManager.h"
#include "Paging/PageManager.h"
#include "Physics/Physics.h"
#include "Network/Network.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

static Engine* gs_engineInstance;
Engine* GetEngine() { return gs_engineInstance; }

Engine::Engine()
	: log(nullptr)
	, stream(nullptr)
	, taskPool(nullptr)
	, resourceManager(nullptr)
	, renderDevice(nullptr)
	, inputManager(nullptr)
	, audioDevice(nullptr)
	, physicsManager(nullptr)
	, scriptManager(nullptr)
{
	gs_engineInstance = this;
}

//-----------------------------------//

Engine::~Engine()
{
	for( size_t i = 0; i < subsystems.size(); i++ )
	{
		Subsystem* system = subsystems[i];
		Deallocate(system);
	}
	
	//Deallocate(inputManager);
	Deallocate(physicsManager);
	Deallocate(scriptManager);
	Deallocate(renderDevice);

#ifdef ENABLE_AUDIO_OPENAL
	Deallocate(audioDevice);
#endif

	Deallocate(resourceManager);

	InputDeinitialize();
	RenderDeinitialize();
	ResourcesDeinitialize();
	NetworkDeinitialize();

	TaskPoolDestroy(taskPool);
	StreamDestroy(stream);
	LogDestroy(log);
}

//-----------------------------------//

void Engine::addSubsystem( Subsystem* const subsystem )
{
	LogInfo( "Registering new engine subsystem" );
	
	subsystems.push_back( subsystem );
}

//-----------------------------------//

void Engine::init( bool createWindow )
{
	// Sets up the main logger.
	setupLogger();

	// Creates the task system.
	taskPool = TaskPoolCreate( AllocatorGetThis(), 2 );

	NetworkInitialize();
	ResourcesInitialize();
	InputInitialize();
	RenderInitialize();

	// Creates the resource manager.
	resourceManager = AllocateThis(ResourceManager);
	resourceManager->setTaskPool( taskPool );
	
	// Registers default resource loaders.
	resourceManager->setupResourceLoaders( ResourceLoaderGetType() );

	// Creates the rendering device.
	renderDevice = AllocateThis(RenderDevice);

#ifdef ENABLE_AUDIO_OPENAL
	// Creates the audio device.
	audioDevice = AudioCreateDevice("");

	if( audioDevice )
		audioDevice->createMainContext();
#endif

#ifdef ENABLE_SCRIPTING_LUA
	// Creates the scripting manager.
	scriptManager = AllocateThis(ScriptManager);
#endif
}

//-----------------------------------//

void Engine::setupLogger()
{
	stream = StreamCreateFromFile( AllocatorGetHeap(), "Log.html", StreamMode::Write);
	log = LogCreate( AllocatorGetHeap() );
}

//-----------------------------------//

void Engine::update()
{
	for( size_t i = 0; i < subsystems.size(); i++ )
	{
		Subsystem* system = subsystems[i];
		system->update();
	}

	resourceManager->update();

#ifdef ENABLE_SCRIPTING_LUA
	scriptManager->update();
#endif
}

//-----------------------------------//

void Engine::stepFrame()
{
	AllocatorReset( GetFrameAllocator() );
}

//-----------------------------------//

NAMESPACE_ENGINE_END