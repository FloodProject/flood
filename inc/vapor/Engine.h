/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"
#include "vapor/render/Device.h"
#include "vapor/resources/ResourceManager.h"
#include "vapor/scene/Scene.h"
#include "vapor/audio/Device.h"

namespace vapor {

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
	Engine(bool autoInit = true);
	virtual ~Engine();

	// Gets the device.
	render::Device* getRenderDevice() const { return renderDevice; }

	// Gets the audio device
	audio::Device* getAudioDevice() const { return audioDevice; }

	// Gets the scene interface
	scene::Scene* getSceneManager() const { return sceneNode; }
	
	// Gets the resources manager.
	resources::ResourceManager* getResourceManager() const { return resourceManager; }

	// Gets the main engine logger.
	log::Log* getLog() const { return log; }

	// Sets up the default resource codecs.
	void setupResourceLoaders();

	// Sets up the resource manager
	void setupResourceManager();

	// Sets up the global engine logger.
	void setupLogger(std::string title, std::string file);

	// Sets up the rendering device.
	void setupDevices();

	// Sets up a rendering window
	void setupWindow(std::string title);

protected:

	// Audio device
	audio::Device* audioDevice;

	// Scene root node
	scene::Scene* sceneNode;

	// Rendering device
	render::Device* renderDevice;

	// Resource manager
	resources::ResourceManager* resourceManager;

	// Default logger
	log::Log* log;
};

} // end namespace