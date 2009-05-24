#pragma once

#include "vapor/RenderQueue.h"
#include "vapor/RenderDevice.h"

namespace vapor {
	namespace render {

class Renderer
{

public:

	Renderer();
	~Renderer();

	Window& getWindow();
	RenderQueue& getRenderQueue();
	RenderDevice& getRenderDevice();

	void render();

private:

	RenderQueue* renderQueue;
	RenderDevice* renderDevice;
};

} } // end namespaces