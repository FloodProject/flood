/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Engine/API.h"
#include "Core/References.h"

FWD_DECL_INTRUSIVE(Scene)

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

struct Stream;
struct TaskPool;

class Subsystem;
class ResourceManager;
class RenderDevice;
class AudioDevice;
class PhysicsManager;
class PageManager;
class InputManager;
class ScriptManager;

//-----------------------------------//

/**
 * Main engine class. This is a utility class that instantiates all the
 * other engine classes and provides suitable methods to get/set them.
 * It is also responsible to set the default logger for all of the engine
 * logging needs. If you need extra flexibility in your app, you can create
 * a class like this one that instantiates everything and sets it all up.
 */

class API_ENGINE Engine
{
public:
	
	Engine();	
	~Engine();

	// Initialize the engine subsystems.
	void init( bool createWindow = true );

	// Updates the main subsystems.
	void update();

	// Adds a subsystem to be managed by the engine.
	void addSubsystem( Subsystem* const subsystem );

	// Sets up the global engine logger.
	void setupLogger();

	// Gets the device.
	GETTER(RenderDevice, RenderDevice*, renderDevice)

	// Gets the audio device.
	GETTER(AudioDevice, AudioDevice*, audioDevice)

	// Gets the device.
	GETTER(TaskPool, TaskPool*, taskPool)

	// Gets the scripting state.
	GETTER(ScriptManager, ScriptManager*, scriptManager)

	// Gets the resources manager.
	GETTER(ResourceManager, ResourceManager*, resourceManager)

	// Gets/sets the physics manager.
	ACESSOR(PhysicsManager, PhysicsManager*, physicsManager)

	// Gets/sets the input manager.
	ACESSOR(InputManager, InputManager*, inputManager)

	// Gets the main engine logger.
	GETTER(Logger, Log*, log)

	void stepFrame();

protected:

	// Subsystems.
	std::vector<Subsystem*> subsystems;

	// Manages background tasks.
	TaskPool* taskPool;

	// Default logger.
	Log* log;

	// Log stream.
	Stream* stream;

	// Rendering device.
	RenderDevice* renderDevice;

	// Input manager.
	InputManager* inputManager;

	// Audio device.
	AudioDevice* audioDevice;

	// Resource manager.
	ResourceManager* resourceManager;

	// Physics manager.
	PhysicsManager* physicsManager;

	// Scripting state.
	ScriptManager* scriptManager;
};

// Gets the engine instance.
API_ENGINE Engine* GetEngine();

//-----------------------------------//

NAMESPACE_ENGINE_END