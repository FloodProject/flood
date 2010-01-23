/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"

#include "vapor/vfs/VFS.h"
#include "vapor/render/Device.h"
#include "vapor/resources/ResourceManager.h"
#include "vapor/scene/Scene.h"
#include "vapor/audio/Device.h"
#include "vapor/input/InputManager.h"
#include "vapor/script/State.h"
#include "vapor/physics/Physics.h"

namespace vapor {

/** \addtogroup Main */
/** @{ */

//-----------------------------------//

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
	render::Device* getRenderDevice() const { return renderDevice; }

#ifdef VAPOR_AUDIO_OPENAL

	/// Gets the audio device.
	audio::Device* getAudioDevice() const { return audioDevice; }

#endif

	/// Gets the scene interface.
	scene::ScenePtr getSceneManager() const { return sceneNode; }

	/// Gets the scripting state.
	script::State* getScriptState() const { return scriptState; }

	/// Gets the resources manager.
	resources::ResourceManager* getResourceManager() const { return resourceManager; }

	/// Gets the physics manager.
	physics::PhysicsManager* getPhysicsManager() const { return physicsManager; }

	/// Gets the main engine logger.
	log::Log* getLog() const { return log; }

	/// Gets the virtual filesystem.
	vfs::VFS* getVFS() const { return vfs; }

	/// Gets the input manager.
	input::InputManager* getInputManager() const { return &( renderDevice->getWindow()->getInputManager() ); }

	/// @}

	/// Updates the main subsystems.
	void update( double delta );

protected:

#ifdef VAPOR_AUDIO_OPENAL

	/// Audio device
	audio::Device* audioDevice;

#endif

	/// Scene root node
	scene::ScenePtr sceneNode;

	/// Rendering device
	render::Device* renderDevice;

	/// Resource manager
	resources::ResourceManager* resourceManager;

	/// Physics manager
	physics::PhysicsManager* physicsManager;

	/// Virtual filesystem
	vfs::VFS* vfs;

	/// Default logger
	log::Log* log;

	/// Scripting state
	script::State* scriptState;

	/// Application name
	std::string app;
	
	/// Arguments
	const char** argv;
};

//-----------------------------------//

/** @} */

} // end namespace