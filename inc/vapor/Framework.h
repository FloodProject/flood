/************************************************************************
*
* vaporEngine by triton © (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include <vapor/Engine.h>

#include <vapor/render/Device.h>
#include <vapor/render/Window.h>

#include <vapor/resources/ResourceManager.h>

using namespace vapor::resources;
using namespace vapor::render;

namespace vapor {
	namespace framework {

 /**
 * Simple framework to facilitate the use of the engine functionality in applications.
 * It asks for a rendering device to use, and has some virtual methods used to setup  all
 * the needed resources. Applications can just derive from this class and setup the needed
 * stuff, call the run() method and everything should automagically work.
 */

class Framework
{

public:

	/**
	 * Kickstart the framework in action.
	 */
	void run();

protected:

	Framework();
	virtual ~Framework();

	/**
	 * Called once when the app starts.
	 */
	virtual void onInit() = 0;

	/**
	 * Sets up all the needed resources.
	 */
	virtual void onSetupResources() = 0;

	/**
	 * Renders the scene (called once per frame).
	 */
	virtual void onRender() = 0;

	// Gets the device.
	Device& getDevice() { return *device; }

private:

	/**
	 * Initializes the engine.
	 */
	virtual void init();

	/**
	 * Main rendering loop ( calls onRender() ).
	 */
	virtual void render();

	/**
	 * Cleans up all the resources.
	 */
	virtual void cleanup();

	Device* device;
	Window* window;
	ResourceManager* resourceManager;
};

} } // end namespaces