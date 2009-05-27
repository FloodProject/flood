/************************************************************************
*
* vaporEngine by triton © (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/Engine.h"

#ifdef VAPOR_IMAGE_PICOPNG
	#include "vapor/resources/PNG_Loader.h"
#endif

#ifdef VAPOR_MESH_MILKSHAPE3D
	#include "vapor/resources/MS3D_Loader.h"
#endif

namespace vapor {

//-----------------------------------//

Engine::Engine()
{
	// create the resource manager
	resourceManager = new ResourceManager();
}

//-----------------------------------//

void Engine::setupLogger(string title, string file)
{
	log = new Log(title, file);
	Log::setLogger(log);
}

//-----------------------------------//

void Engine::setupDevice()
{
	// create render device
	device = Device::createDevice();

	// set the window title
	device->getWindow()->setTitle("vaporEngine Example");
}

//-----------------------------------//

void Engine::setupResourceLoaders()
{
	ResourceManager* rm = getResourceManager();
	ResourceLoader* handler = nullptr;

	// register default compiled codecs
	#ifdef VAPOR_IMAGE_PICOPNG
		handler = new PNG_Pico_Loader();
		rm->registerResourceLoader(handler);
	#endif

	#ifdef VAPOR_MESH_MILKSHAPE3D
		handler = new MS3D_Loader();
		rm->registerResourceLoader(handler);	
	#endif
}

//-----------------------------------//

Engine::~Engine()
{
	// delete stuff
	delete device;
	delete resourceManager;
	delete log::Log::getLogger();
}

} // end namespace