/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Engine/API.h"
#include "Input/Mouse.h"
#include "Input/Keyboard.h"
#include "Graphics/FrameStatistics.h"
#include "Core/Timer.h"
#include "Engine/Engine.h"
#include "Window/Window.h"
#include "Scene/Scene.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

class Window;
struct Archive;

 /**
 * Simple framework to facilitate the use of the engine functionality in
 * applications. It asks for a rendering device to use, and has some 
 * virtual methods used to setup all the needed resources. Applications 
 * can just derive from this class and setup the needed stuff, call the 
 * run() method and everything should automagically work.
 */

class API_ENGINE Framework : public Engine
{
public:

	Framework(const String& app);
	virtual ~Framework();

	// Initializes the engine.
	void init();

	// Kickstart the framework in action.
	void run();

	// Sets up the render window.
	void setupWindow(Window* window);

	// Sets up the resource paths.
	void setupResourcePaths();

protected:

	// Called once when the app starts.
	virtual void onInit() = 0;

	// Called once when the app exits.
	virtual void onCleanup() = 0;

	// Sets up all the needed resources.
	virtual void onSetupResources() = 0;

	// Sets up all the scene nodes.
	virtual void onSetupScene() = 0;

	// Updates the scene (called once per frame).
	virtual void onUpdate( float delta ) = 0;

	// Renders the scene (called once per frame).
	virtual void onRender() = 0;

	// Input callback functions.
	virtual void onKeyPressed( const KeyEvent& ) {}
	virtual void onKeyReleased( const KeyEvent& ) {}
	virtual void onButtonPressed( const MouseButtonEvent& ) {}
	virtual void onButtonReleased( const MouseButtonEvent& ) {}

	// Register input devices callbacks.
	void registerCallbacks();

	// Calculates some statistics about frame times.
	void updateFrameTimes();

	// Main rendering loop.
	virtual void mainLoop();

public:

	// Resources archive.
	Archive* archive;

	// Main window;
	Window* window;

	// Main scene.
	SceneHandle sceneHandle;

	// Frame loop timer.
	Timer frameTimer;

	// Holds frame statistics.
	FrameStatistics frameStats;
};

//-----------------------------------//

NAMESPACE_ENGINE_END
