/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Runtime/API.h"
#include "Runtime/Runtime.h"
#include "Engine/Scene/Scene.h"
#include "Core/Timer.h"
#include "Graphics/FrameStatistics.h"

NAMESPACE_GAMERUNTIME_BEGIN

class Engine;
class Window;
class Archive;

class API_GAMERUNTIME Game : public Runtime
{
public:
	Game(uint16 hz = 120);
	virtual ~Game();

	void run();

protected:
	void frame();

	virtual void init();
	virtual void loadContent();
	virtual void shutdown();

	virtual void update(float delta) {}
	virtual void render() {}

public:
	Engine * engine;

	Window * window;
	Archive * archive;
	SceneHandle scene;

	bool running;
	uint16 hertz;
	Timer frameTimer;
	FrameStatistics frameStats;
};

Game * game();

NAMESPACE_GAMERUNTIME_END