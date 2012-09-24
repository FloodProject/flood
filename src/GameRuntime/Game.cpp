/************************************************************************
*
* vapor3D Engine © (2008-2013)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "GameRuntime/API.h"
#include "Game.h"
#include "Engine/Engine.h"
#include "Core/Archive.h"
#include "Engine/Window/Window.h"
#include "Engine/Input/InputManager.h"
#include "Platforms/SFML/SFML_Window.h"
#include "Graphics/RenderDevice.h"
#include "Graphics/RenderContext.h"
#include "Resources/ResourceManager.h"

NAMESPACE_GAMERUNTIME_BEGIN

namespace 
{
	Game * g_Game = nullptr;
}

Game * game()
{
	return g_Game;
}

Game::Game(uint16 hz)
	: Runtime()
	, engine(nullptr)
	, window(nullptr)
	, archive(nullptr)
	, scene()
	, hertz(hz)
	, running(true)
{
	// there should only be a single Game instance ever created
	assert(g_Game == nullptr);
	LogInfo("Game runtime created.");

	g_Game = this;
}

Game::~Game()
{
	g_Game = nullptr;
}

void Game::init()
{
	Runtime::init();

	// initialize global engine instance
	engine = AllocateThis(Engine);
	engine->init();

	// make up some window settings & allocate
	// TODO: move this to command line arguments or config file
	WindowSettings ws(800, 600, "Flood", false);
	SFML_Window* sfWindow = AllocateHeap(SFML_Window, ws);

	assert(sfWindow != nullptr);

	// "opening" the window actually creates the window so we can draw to it
	sfWindow->open();
	window = sfWindow;

	// hook the window into the engine as the input source and back buffer
	window->getInput()->createDefaultDevices();
	engine->setInputManager( window->getInput() );

	RenderDevice * dev = engine->getRenderDevice();
	dev->setRenderTarget( window );

	RenderContext * context = window->getContext();
	assert( context != nullptr );
	context->init();

	// create and mount the archive
	archive = ArchiveCreateVirtual( AllocatorGetHeap() );
	engine->getResourceManager()->setArchive(archive);
}

void Game::loadContent()
{}

void Game::shutdown()
{
	ArchiveDestroy(archive);
	Deallocate(window);

	//Runtime::shutdown();
}

void Game::frame()
{
	const float maxDelta = 1.0f / hertz;

	float tick = TimerGetCurrentTimeMs();		// Time for the whole frame
	float delta = TimerGetElapsed(&frameTimer);	// Time for the frame delta
	TimerReset(&frameTimer);

	do
	{
		// NOTE: The value in delta on the first iteration should be
		// equivalent to the draw time of the last frame.
		update(delta);

		engine->update();

		delta = TimerGetElapsed(&frameTimer);
	} while(delta < (maxDelta / 2.f));

	// Reset frame timer to read time since last opportunistic update finished
	TimerReset(&frameTimer);

	render();

	// Swap buffers
	window->update();

	engine->stepFrame();

	tick = TimerGetCurrentTimeMs() - tick;
	frameStats.lastFrameTime = tick;
	frameStats.frameStep();
}

void Game::run()
{
	init();
	loadContent();

	while(running && window->pumpEvents())
		frame();

	shutdown();
}

NAMESPACE_GAMERUNTIME_END