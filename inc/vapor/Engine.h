/************************************************************************
*
* vaporEngine (2008-2009)
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

namespace vapor {

#ifndef VAPOR_AUDIO_OPENAL
	// declare audio namespace so using decls works...
	namespace audio {}
#endif

/**
 * Main engine class. This is a utility class that instantiates all the
 * other engine classes and provides suitable methods to get/set them.
 * It is also responsible to set the default logger for all of the engine
 * logging needs. If you need extra flexibility in your app, you can create
 * a class like this one that instantiates everything and sets it all up.
 */

class Engine
{
public:

	// If autoInit is true, then the contructor will make sure
	// everything is properly set up when you instantiate the object,
	// else you will have to call the methods manually, to initialize.
	Engine(std::string app = "vaporApp", bool autoInit = true);
	virtual ~Engine();

	//-----------------------------------//
	// Setup methods
	//-----------------------------------//

	// Initialize the engine subsystems.
	void init();

	// Sets up the global engine logger.
	void setupLogger();

	// Sets up the resource manager.
	void setupResourceManager();

	// Sets up the default resource codecs.
	void setupResourceLoaders();

	// Sets up the rendering device.
	void setupDevices();

	//-----------------------------------//
	// Subsystem acessors
	//-----------------------------------//

	// Gets the device.
	render::Device* getRenderDevice() const { return renderDevice; }

#ifdef VAPOR_AUDIO_OPENAL

	// Gets the audio device.
	audio::Device* getAudioDevice() const { return audioDevice; }

#endif

	// Gets the scene interface.
	scene::Scene* getSceneManager() const { return sceneNode; }
	
	// Gets the resources manager.
	resources::ResourceManager* getResourceManager() const { return resourceManager; }

	// Gets the main engine logger.
	log::Log* getLog() const { return log; }

	// Gets the virtual filesystem.
	vfs::VFS* getVFS() const { return vfs; }

protected:

#ifdef VAPOR_AUDIO_OPENAL

	// Audio device
	audio::Device* audioDevice;

#endif

	// Scene root node
	scene::Scene* sceneNode;

	// Rendering device
	render::Device* renderDevice;

	// Resource manager
	resources::ResourceManager* resourceManager;

	// Virtual filesystem
	vfs::VFS* vfs;

	// Default logger
	log::Log* log;

	// Application name
	std::string app;
};

} // end namespace