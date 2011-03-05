/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/PCH.h"
#include "ReferenceCount.h"

FWD_DECL_SHARED(Scene)

namespace vapor {

//-----------------------------------//

class Subsystem;
class TaskManager;
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

class VAPOR_API Engine
{
public:
	
	Engine();	
	~Engine();

	// Initialize the engine subsystems.
	void init( bool createWindow = true );

	// If autoInit is true, then the method will make sure everything 
	// is properly set up when you instantiate the object, else you will
	// have to call the init methods manually.
	void create(const std::string& app, const char** argv = nullptr);

	// Adds a subsystem to be managed by the engine.
	void addSubsystem( Subsystem* const subsystem );

	// Sets up the global engine logger.
	void setupLogger();

	// Sets up the rendering device.
	void setupDevices( bool createWindow = true );

	// Sets up the input devices.
	void setupInput();

	// Gets/sets the scene.
	ACESSOR(SceneManager, ScenePtr, scene)

	// Gets the device.
	GETTER(RenderDevice, RenderDevice*, renderDevice)

	// Gets the audio device.
	GETTER(AudioDevice, AudioDevice*, audioDevice)

	// Gets the device.
	GETTER(TaskManager, TaskManager*, taskManager)

	// Gets the scripting state.
	GETTER(ScriptManager, ScriptManager*, scriptManager)

	// Gets the resources manager.
	GETTER(ResourceManager, ResourceManager*, resourceManager)

	// Gets/sets the physics manager.
	ACESSOR(PhysicsManager, PhysicsManager*, physicsManager)

	// Gets the main engine logger.
	GETTER(Logger, Logger*, log)

	// Gets the virtual filesystem.
	GETTER(FileSystem, FileSystem*, fileSystem)

	// Gets the input manager.
	InputManager* getInputManager() const;

	// Updates the main subsystems.
	void update( float delta );

protected:

	// Subsystems.
	std::vector< Subsystem* > subsystems;

	// Manages background tasks.
	TaskManager* taskManager;

	// Default logger.
	Logger* log;

	// Log stream.
	FileStream* stream;

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

	// Application name.
	std::string app;
	
	// Arguments.
	const char** argv;
};

// Gets the engine instance.
Engine* GetEngine();

//-----------------------------------//

} // end namespace