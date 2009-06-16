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

namespace vapor {

using namespace vapor::render;
using namespace vapor::resources;
using namespace vapor::log;

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

	Engine();
	virtual ~Engine();

protected:

	// Gets the device.
	Device* getDevice() const { return device; }
	
	// Gets the resources manager.
	ResourceManager* getResourceManager() const { return resourceManager; }

	// Gets the main engine logger.
	Log* getLog() const { return log; }

	// Sets up the default resource codecs.
	void setupResourceLoaders();

	// Sets up the global engine logger.
	void setupLogger(string title, string file);

	// Sets up the rendering device.
	void setupDevice();

private:

	// Rendering device
	Device* device;

	// Resource manager
	ResourceManager* resourceManager;

	// Default logger
	Log* log;
};

} // end namespace