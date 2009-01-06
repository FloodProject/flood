/************************************************************************
*
* vaporEngine by triton (2008)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include <vapor/Engine.h>

#include <vapor/render/Device.h>
#include <vapor/render/Window.h>

#include <vapor/resources/ResourceManager.h>
#include <vapor/resources/Milkshape3D.h>
#include <vapor/resources/TTF.h>

using namespace vapor::resources;
using namespace vapor::render;

#include "Dialog.h"

class EngineFramework
{

private:

	Device *device;
	ResourceManager *resourceManager;

public:

	void ask()
	{
		Dialog *dialog = new Dialog();

		delete dialog;
	}

	void setup()
	{
		// create render device
		device = Device::createDevice();

		// set the window title
		device->getWindow().setTitle("vaporEngine Example");

		// create the resource manager
		resourceManager = new ResourceManager();
	}

	void resources()
	{
		// add milkshape resource
		resourceManager->addResource("cube", new Milkshape3D("media/cubo.ms3d"));

		// load a ttf font
		resourceManager->addResource("arial", new TTF("arial.ttf"));
	}

	void render()
	{
		do {

			// clear the render device
			device->clearTarget();

			// update the active target
			device->updateTarget();

		} while( device->getWindow().pump() ); 
	}

	void cleanup()
	{
		// free all objects

		delete device;
		delete resourceManager;
		delete Log::getLogger();
	}

	void run()
	{
		// create settings dialog
		//ask();

		// set up the engine
		setup();

		// set up all the resources
		resources();
	
		// main render loop
		render();

		// clean up time
		cleanup();
	}
};
