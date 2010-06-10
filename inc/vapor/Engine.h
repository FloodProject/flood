/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/PCH.h"
#include "vapor/Singleton.h"

FWD_DECL(audio, Device)
FWD_DECL_TYPEDEF_PTR(Subsystem)
FWD_DECL_TYPEDEF_PTR(TaskManager)
FWD_DECL_TYPEDEF_PTR(ResourceManager)
FWD_DECL_TYPEDEF_PTR(RenderDevice)
FWD_DECL_TYPEDEF_SHARED(Scene)

namespace vapor {

class InputManager;
class FileSystem;
class State;

/** \addtogroup Main */
/** @{ */

/**
 * Main engine class. This is a utility class that instantiates all the
 * other engine classes and provides suitable methods to get/set them.
 * It is also responsible to set the default logger for all of the engine
 * logging needs. If you need extra flexibility in your app, you can create
 * a class like this one that instantiates everything and sets it all up.
 */

class VAPOR_API Engine : public Singleton<Engine>
{
	friend class Singleton<Engine>;

public:
	
	~Engine();

	/// \name Setup methods
	/// Use these methods to setup the engine subsystems.
	/// @{

	// If autoInit is true, then the method will make sure everything 
	// is properly set up when you instantiate the object, else you will
	// have to call the init methods manually.
	void create(const std::string& app = "vaporApp", 
		const char** argv = nullptr, bool autoInit = true);

	/// Initialize the engine subsystems.
	void init( bool createWindow = true );

	/// Sets up the global engine logger.
	void setupLogger();

	/// Sets up the resource manager.
	void setupResourceManager();

	/// Sets up the default resource codecs.
	void setupResourceLoaders();

	/// Sets up the rendering device.
	void setupDevices( bool createWindow = true );

	/// Sets up the input devices.
	void setupInput();

	/// @}

	/// \name Subsystem acessors
	/// Use these methods to access the engine subsystems.
	/// @{

	/// Gets the device.
	IMPLEMENT_GETTER(RenderDevice, RenderDevicePtr, renderDevice)

	/// Gets the audio device.
#ifdef VAPOR_AUDIO_OPENAL
	IMPLEMENT_GETTER(AudioDevice, audio::Device*, audioDevice)
#endif

	/// Gets the scene interface.
	IMPLEMENT_GETTER(SceneManager, ScenePtr, sceneManager)

	/// Gets the device.
	IMPLEMENT_GETTER(TaskManager, TaskManagerPtr, taskManager)

	/// Gets the scripting state.
	IMPLEMENT_GETTER(ScriptState, State*, scriptState)

	/// Gets the resources manager.
	IMPLEMENT_GETTER(ResourceManager, ResourceManagerPtr, resourceManager)

	/// Gets the physics manager.
	//IMPLEMENT_GETTER(PhysicsManager, physics::PhysicsManager*, physicsManager)

	/// Gets the main engine logger.
	IMPLEMENT_GETTER(Log, Log*, log)

	/// Gets the virtual filesystem.
	IMPLEMENT_GETTER(FileSystem, FileSystem*, fileSystem)

	/// Gets the input manager.
	InputManager* const getInputManager() const;

	/// @}

	/// Updates the main subsystems.
	void update( double delta );

protected:

	Engine();

	// Subsystems
	std::vector< SubsystemPtr > subsystems;

#ifdef VAPOR_AUDIO_OPENAL
	/// Audio device
	audio::Device* audioDevice;
#else
	void* audioDevice;
#endif

	/// Manages background tasks.
	TaskManagerPtr taskManager;

	/// Scene root node.
	ScenePtr sceneManager;

	/// Rendering device.
	RenderDevicePtr renderDevice;

	/// Resource manager.
	ResourceManagerPtr resourceManager;

	/// Physics manager.
	//physics::PhysicsManager* physicsManager;

	/// Virtual filesystem.
	FileSystem* fileSystem;

	/// Default logger.
	Log* log;

	/// Scripting state.
	State* scriptState;

	/// Application name.
	std::string app;
	
	/// Arguments.
	const char** argv;
};

//-----------------------------------//

/** @} */

} // end namespace