/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

FWD_DECL(vfs, VFS)
FWD_DECL(script, State)
FWD_DECL(render, Device)
FWD_DECL(audio, Device)
FWD_DECL(input, InputManager)
FWD_DECL(physics, PhysicsManager)
FWD_DECL(resources, ResourceManager)
FWD_DECL_TYPEDEF_SHARED(scene, Scene)

namespace vapor {

class TaskManager;
class Subsystem;

/** \addtogroup Main */
/** @{ */

/**
 * Main engine class. This is a utility class that instantiates all the
 * other engine classes and provides suitable methods to get/set them.
 * It is also responsible to set the default logger for all of the engine
 * logging needs. If you need extra flexibility in your app, you can create
 * a class like this one that instantiates everything and sets it all up.
 */

class VAPOR_API Engine : private boost::noncopyable
{
public:

	// If autoInit is true, then the contructor will make sure
	// everything is properly set up when you instantiate the object,
	// else you will have to call the methods manually, to initialize.
	Engine(const std::string& app = "vaporApp", const char** argv = nullptr,
		bool autoInit = true);
	
	/// Destructor.
	virtual ~Engine();

	/// \name Setup methods
	/// Use these methods to setup the engine subsystems.
	/// @{

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
	IMPLEMENT_GETTER(RenderDevice, render::Device*, renderDevice)

	/// Gets the audio device.
#ifdef VAPOR_AUDIO_OPENAL
	IMPLEMENT_GETTER(AudioDevice, audio::Device*, audioDevice)
#endif

	/// Gets the scene interface.
	IMPLEMENT_GETTER(SceneManager, scene::ScenePtr, sceneManager)

	/// Gets the device.
	IMPLEMENT_GETTER(TaskManager, TaskManager*, taskManager)

	/// Gets the scripting state.
	IMPLEMENT_GETTER(ScriptState, script::State*, scriptState)

	/// Gets the resources manager.
	IMPLEMENT_GETTER(ResourceManager, resources::ResourceManager*, resourceManager)

	/// Gets the physics manager.
	IMPLEMENT_GETTER(PhysicsManager, physics::PhysicsManager*, physicsManager)

	/// Gets the main engine logger.
	IMPLEMENT_GETTER(Log, log::Log*, log)

	/// Gets the virtual filesystem.
	IMPLEMENT_GETTER(VFS, vfs::VFS*, vfs)

	/// Gets the input manager.
	input::InputManager* const getInputManager() const;

	/// @}

	/// Updates the main subsystems.
	void update( double delta );

protected:

	// Subsystems
	std::vector< Subsystem* > subsystems;

#ifdef VAPOR_AUDIO_OPENAL

	/// Audio device
	audio::Device* audioDevice;

#endif

	/// Manages background tasks.
	TaskManager* taskManager;

	/// Scene root node.
	scene::ScenePtr sceneManager;

	/// Rendering device.
	render::Device* renderDevice;

	/// Resource manager.
	resources::ResourceManager* resourceManager;

	/// Physics manager.
	physics::PhysicsManager* physicsManager;

	/// Virtual filesystem.
	vfs::VFS* vfs;

	/// Default logger.
	log::Log* log;

	/// Scripting state.
	script::State* scriptState;

	/// Application name.
	std::string app;
	
	/// Arguments.
	const char** argv;
};

//-----------------------------------//

/** @} */

} // end namespace