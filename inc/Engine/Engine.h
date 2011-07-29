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
class FileSystem;
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

	// Adds a subsystem to be managed by the engine.
	void addSubsystem( Subsystem* const subsystem );

	// Sets up the global engine logger.
	void setupLogger();

	// Sets up the rendering device.
	void setupDevices( bool createWindow = true );

	// Sets up the input devices.
	void setupInput();

	// Gets/sets the scene.
	GETTER(Scene, ScenePtr, scene)
	void setScene(const ScenePtr& scene);

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

	// Gets the main engine logger.
	GETTER(Logger, Log*, log)

	// Gets the virtual filesystem.
	GETTER(FileSystem, FileSystem*, fileSystem)

	// Gets the input manager.
	InputManager* getInputManager() const;

	// Updates the main subsystems.
	void update( float delta );

	void stepFrame();

protected:

	// Subsystems.
	std::vector< Subsystem* > subsystems;

	// Manages background tasks.
	TaskPool* taskPool;

	// Default logger.
	Log* log;

	// Log stream.
	Stream* stream;

	// Scene root node.
	ScenePtr scene;

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

	// Virtual filesystem.
	FileSystem* fileSystem;

	// Scripting state.
	ScriptManager* scriptManager;
};

// Gets the engine instance.
API_ENGINE Engine* GetEngine();

//-----------------------------------//

NAMESPACE_ENGINE_END