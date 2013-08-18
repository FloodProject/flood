/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Engine/API.h"
#include "Engine/Engine.h"

#include "Core/Memory.h"
#include "Core/Network/Network.h"
#include "Resources/ResourceManager.h"
#include "Resources/ResourceLoader.h"
#include "Graphics/Texture.h"
#include "Graphics/RenderDevice.h"
#include "Engine/Input/InputManager.h"
#include "Engine/Audio/Device.h"
#include "Engine/Paging/PageManager.h"
#include "Engine/Physics/Physics.h"
#include "Engine/Window/WindowManager.h"
#include "Script/ScriptManager.h"

#ifdef API_ENGINE_DLL_EXPORT
FL_INSTANTIATE_TEMPLATES()
#endif

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
	, windowManager(nullptr)
{
	assert(gs_engineInstance == nullptr);
	gs_engineInstance = this;
}

//-----------------------------------//

Engine::~Engine()
{
	for(auto& subsystem : subsystems)
    {
		Deallocate(subsystem);
    }
	
	Deallocate(physicsManager);
	Deallocate(scriptManager);
	Deallocate(renderDevice);
	Deallocate(windowManager);

#ifdef ENABLE_AUDIO_OPENAL
	Deallocate(audioDevice);
#endif

	Deallocate(resourceManager);

	InputDeinitialize();
	GraphicsDeinitialize();
	ResourcesDeinitialize();

#ifdef ENABLE_NETWORK_ENET
	NetworkDeinitialize();
#endif

	Deallocate(taskPool);
	Deallocate(stream);
	LogDestroy(log);
}

//-----------------------------------//

void Engine::addSubsystem( Subsystem* const subsystem )
{
	LogInfo( "Registering new engine subsystem" );
	
	subsystems.push_back( subsystem );
}

//-----------------------------------//

void Engine::init()
{
	// Sets up the main logger.
	setupLogger();

	// Creates the task system.
	taskPool = AllocateThis(TaskPool, 2 );

#ifdef ENABLE_NETWORK_ENET
	NetworkInitialize();
#endif

	ResourcesInitialize();
	InputInitialize();
	GraphicsInitialize();

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
	stream = AllocateHeap(FileStream, "Log.html", StreamOpenMode::Write);
	log = LogCreate( AllocatorGetHeap() );
}

//-----------------------------------//

void Engine::update()
{
	for(auto& subsystem : subsystems)
		subsystem->update();

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