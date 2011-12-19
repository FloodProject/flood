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
#include "Graphics/RenderDevice.h"
#include "Graphics/RenderContext.h"
#include "Scene/Scene.h"
#include "Input/InputManager.h"
#include "Resources/ResourceManager.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

Framework::Framework(const String& app)
	: window(nullptr)
	, numUpdatesSecond(30)
{
	LogInfo( "Engine framework getting into action" );
}

//-----------------------------------//

Framework::~Framework()
{

}

//-----------------------------------//

void Framework::init()
{
	// Init the engine.
	Engine::init(true);

	// Sets up the main window.
	setupWindow();
}

//-----------------------------------//

void Framework::cleanup()
{
	ArchiveDestroy(archive);
	Deallocate(window);
}

//-----------------------------------//

void Framework::setupWindow()
{
	assert( window != nullptr );

	// Input setup.
	window->getInput()->createDefaultDevices();
	setInputManager( window->getInput() );

	// Setup the window as a target.
	RenderDevice* device = getRenderDevice();
	device->setRenderTarget( window );

	// Initializes the render device with new window context.
	RenderContext* context = window->getContext();
	assert( context != nullptr );

	context->init();

	// Register input callbacks.
	registerCallbacks();
}

//-----------------------------------//

void Framework::setupResourcePaths()
{
	// Create the archive and mount the paths.
	archive = ArchiveCreateVirtual( AllocatorGetHeap() );

	// Set as the resource archive.
	ResourceManager* res = getResourceManager();
	res->setArchive(archive);
}

//-----------------------------------//

void Framework::run()
{
	// User resources setup callback.
	onSetupResources();

	// User scene setup callback.
	onSetupScene();
}

//-----------------------------------//

/**
 * Check out this article for a detailed analysis of some possible 
 * game loop implementations: http://dewitters.koonsolo.com/gameloop.html
 */

void Framework::mainLoop()
{
	const float maxUpdateTime = 1.0f / numUpdatesSecond;

	Timer updateTimer;
	TimerReset(&updateTimer);

	float nextTick = TimerGetCurrentTimeMs();

	TimerReset(&frameTimer);
		
	do
	{
		// User update callback.
		onUpdate( maxUpdateTime );

		update();

		nextTick += maxUpdateTime;
	}
	while( TimerGetCurrentTimeMs() > nextTick );

	// User rendering callback.
	onRender();

	// Update the active target (swaps buffers).
	window->update();

	stepFrame();

	// Calculates the new frame times.
	updateFrameTimes();
}

//-----------------------------------//

void Framework::updateFrameTimes()
{
	frameStats.lastFrameTime = TimerGetElapsed(&frameTimer);

	if(frameStats.lastFrameTime > 0.1)
		LogDebug("Hotspot frame! %lf", frameStats.lastFrameTime);

	frameStats.frameStep();
}

//-----------------------------------//

void Framework::registerCallbacks()
{
	InputManager* input = getInputManager();

	if( !input ) return;

	Keyboard* keyboard = input->getKeyboard();
	keyboard->onKeyPress.Connect( this, &Framework::onKeyPressed );
	keyboard->onKeyRelease.Connect( this, &Framework::onKeyReleased );
	
	Mouse* mouse = input->getMouse();
	mouse->onMouseButtonPress.Connect( this, &Framework::onButtonPressed );
	mouse->onMouseButtonRelease.Connect( this, &Framework::onButtonReleased );
}

//-----------------------------------//

NAMESPACE_ENGINE_END