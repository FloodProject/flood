/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include <vapor/PCH.h>
#include <vapor/Framework.h>
#include <vapor/render/Device.h>
#include <vapor/vfs/FileSystem.h>
#include <vapor/input/InputManager.h>
#include <vapor/resources/ResourceManager.h>

namespace vapor {

//-----------------------------------//

Framework::Framework(const std::string& app, const char** argv)
{
	info( "framework", "Engine framework getting into action" );
	Engine::create(app, argv, false);
}

//-----------------------------------//

void Framework::run()
{
	// init the engine
	Framework::init();

	// call the render cycle
	render();
}

//-----------------------------------//

void Framework::init()
{
	// Init the engine.
	Engine::init( true );

	// Register default media locations.
	FileSystem* fs = getFileSystem();
	fs->mountDefaultLocations();

	// Register input callbacks.
	registerCallbacks();

	// User init callback.
	onInit();

	// User resources setup callback.
	onSetupResources();

	// User scene setup callback.
	onSetupScene();

	// Wait until all resources are loaded.
	ResourceManager* rm = getResourceManager();
	rm->waitUntilQueuedResourcesLoad();
}

//-----------------------------------//

/**
 * Check out this article for a detailed analysis of some possible 
 * game loop implementations: http://dewitters.koonsolo.com/gameloop.html
 */

void Framework::render()
{
	RenderDevice* renderDevice = getRenderDevice();
	Window* window = renderDevice->getWindow();

	while( frameTimer.reset(), window->pumpEvents() )
	{
		{
			//PROFILE_STR("Engine update");
			update( frameStats.lastFrameTime );
		}

		// User update callback.
		onUpdate( frameStats.lastFrameTime );

		{
			//PROFILE_STR("Engine render");
			// User rendering callback.
			onRender();
		}

		// Update the active target (swaps buffers).
		window->update();

		// Calculates the new frame times.
		updateFrameTimes();
	}

	Timer::sleep(0);
}

//-----------------------------------//

void Framework::registerCallbacks()
{
	InputManager* im = getInputManager();

	Keyboard* keyboard = im->getKeyboard();

	if( keyboard )
	{
		keyboard->onKeyPress +=
			fd::bind( &Framework::onKeyPressed, this );
	}
	
	Mouse* mouse = im->getMouse();

	if( mouse )
	{
		mouse->onMouseButtonPress +=
			fd::bind( &Framework::onButtonPressed, this );
		
		mouse->onMouseButtonRelease +=
			fd::bind( &Framework::onButtonReleased, this );
	}
}

//-----------------------------------//

void Framework::updateFrameTimes()
{
	frameStats.lastFrameTime = frameTimer.getElapsedTime();

	if(frameStats.lastFrameTime > 0.1)
		debug("HOTSPOT! %lf", frameStats.lastFrameTime);

	frameStats.frameStep();
}

//-----------------------------------//

void Framework::onKeyPressed( const KeyEvent& )
{

}

//-----------------------------------//

void Framework::onButtonPressed( const MouseButtonEvent& )
{

}

//-----------------------------------//

void Framework::onButtonReleased( const MouseButtonEvent& )
{

}

//-----------------------------------//

} // end namespace