/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/CompileOptions.h"

#ifdef VAPOR_RENDERER_OPENGL

#include "vapor/Platform.h"

#include "vapor/render/Window.h"
#include "vapor/render/Queue.h"
#include "vapor/render/Target.h"
#include "vapor/render/Adapter.h"
#include "vapor/render/BufferManager.h"
#include "vapor/render/VertexBuffer.h"

#include "vapor/math/Matrix.h"
#include "vapor/math/Color.h"

namespace vapor {
	namespace render {

/**
 * Represents the rendering device we are using. At startup the application
 * can ask the user what device he wants to use (OpenGL / DirectX / Software)
 * and then we create and use the device. All operations are to be done by this
 * abstract class. The user of the engine doesn't need to care about internal
 * details how all the rendering operations are implemented.
 *
 * This class only knows about rendering primitives (VertexBuffer and IndexBuffer)
 * and does not know anything about the high-level meshes.
 *
 * Each render device will also manage a list of render targets (Windows or RTT).
 * Window implementations depend on the rendering system used, for example, an
 * OpenGL window will be different from an DirectX window, so this is also
 * responsible for creating a new window if no window handle is passed to it.
 */

class Device
{
public:

	Device();
	~Device();

	// Create a new render window
	Window& createWindow( Settings& settings = Settings(640, 480) );

	// Gets the main window 
	Window* getWindow() const;

	// Gets rendering adapter information
	Adapter* getAdapter() const;

	// Gets the buffer manager
	BufferManager* getBufferManager() const;

	// Clears the active render target.
	void clearTarget( ) = 0;

	// Updates the target render target.
	void updateTarget( );

	// Sets the current clear color
	void setClearColor(math::Color c);

	// Sets the active render target.
	void setRenderTarget( RenderTarget* renderTarget );

	// Close the device
	void close();

	// Clear buffers
	void clearTarget();

protected:

	// active render target
	RenderTarget* activeTarget;

	// list of render targets
	vector<RenderTarget*> renderTargets;

	// adapter information
	Adapter* adapter;

	// render window
	Window* window;

	// manages all buffers
	BufferManager* bufferManager;

	// current clear color
	math::Color clearColor;

private:

	void resetViewport();

	void checkExtensions();
};

} } // end namespaces

#endif