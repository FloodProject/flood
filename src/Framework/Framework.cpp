/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"
#include "Framework.h"
#include "Core/Archive.h"
#include "Core/Timer.h"
#include "Core/Utilities.h"
#include "Render/Device.h"
#include "Scene/Scene.h"
#include "Input/InputManager.h"
#include "Resources/ResourceManager.h"

#if defined(VAPOR_WINDOWING_WIN32)
	#include <Render/Win32_Window.h>
#elif defined(VAPOR_WINDOWING_SFML)
	#include <Render/SFML_Window.h>
#endif

namespace vapor {

//-----------------------------------//

Framework::Framework(const std::string& app, const char** argv)
{
	LogInfo( "Engine framework getting into action" );
	Engine::create(app);
}

//-----------------------------------//

void Framework::run()
{
	Framework::init();
	mainLoop();
}

//-----------------------------------//

void Framework::init()
{
	// Init the engine.
	Engine::init(true);

	// Creates the window.
	createWindow();

	// Mount the default assets path.
	ResourceManager* res = getResourceManager();
	Archive* archive = ArchiveCreateVirtual( GetResourcesAllocator() );
	ArchiveMountDirectories(archive, "Assets", GetResourcesAllocator());
	res->setArchive(archive);

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
	rm->loadQueuedResources();
}

//-----------------------------------//

void Framework::createWindow()
{
	WindowSettings settings;

	Window* window = nullptr;

	#if defined(VAPOR_WINDOWING_WIN32)
		window = new Win32Window(settings);
	#elif defined(VAPOR_WINDOWING_SFML)
		window = new SFML_Window(settings);
	#else
		#error "Could not find a window implementation"
	#endif

	assert( window != nullptr );

	RenderDevice* device = getRenderDevice();

	device->setWindow( window );
	device->setRenderTarget( window );

	// Initializes the render device with new window context.
	device->init();
}

//-----------------------------------//

/**
 * Check out this article for a detailed analysis of some possible 
 * game loop implementations: http://dewitters.koonsolo.com/gameloop.html
 */

void Framework::mainLoop()
{
	RenderDevice* renderDevice = getRenderDevice();
	Window* window = renderDevice->getWindow();

	const uint16 numUpdatesSecond = 25;
	const float maxUpdateTime = 1.0f / numUpdatesSecond;

	Timer updateTimer;
	TimerReset(&updateTimer);

	float nextTick = TimerGetCurrentTimeMs();

	while( true )
	{
		TimerReset(&frameTimer);
		
		if( !window->pumpEvents() )
			break;

		while( TimerGetCurrentTimeMs() > nextTick )
		{
			update( maxUpdateTime );

			// User update callback.
			onUpdate( maxUpdateTime );

			nextTick += maxUpdateTime;
		}

		// User rendering callback.
		onRender();

		// Update the active target (swaps buffers).
		window->update();

		// Calculates the new frame times.
		updateFrameTimes();
	}

	SystemSleep(0);
}

//-----------------------------------//

void Framework::updateFrameTimes()
{
	frameStats.lastFrameTime = TimerGetElapsed(&frameTimer);

	if(frameStats.lastFrameTime > 0.1)
		LogDebug("HOTSPOT! %lf", frameStats.lastFrameTime);

	frameStats.frameStep();
}

//-----------------------------------//

void Framework::registerCallbacks()
{
	InputManager* input = getInputManager();

	if( !input ) return;

	Keyboard* keyboard = input->getKeyboard();
	keyboard->onKeyPress.Connect( this, &Framework::onKeyPressed );
	
	Mouse* mouse = input->getMouse();
	mouse->onMouseButtonPress.Connect( this, &Framework::onButtonPressed );
	mouse->onMouseButtonRelease.Connect( this, &Framework::onButtonReleased );
}

//-----------------------------------//

} // end namespace