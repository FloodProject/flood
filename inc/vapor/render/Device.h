/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

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

	Device( );
	virtual ~Device( );

	// Creates a new rendering device, using the rendering API specified.
	// TODO: add suport for passing a specific backend
	static Device* createDevice( Settings settings = Settings(640, 480) );
	
	// Clears the active render target.
	virtual void clearTarget( ) = 0;

	// Updates the target render target.
	virtual void updateTarget( );

	// Sets the current clear color
	virtual void setClearColor(math::Color c);

	// Sets the active render target.
	virtual void setRenderTarget( RenderTarget* renderTarget );

	// Create a new window if not passed a window handle
	virtual Window& createWindow( Settings& settings ) = 0;

	// Gets the main window 
	virtual Window* getWindow() const;

	// Gets rendering adapter information
	virtual Adapter* getAdapter() const;

	// Gets the buffer manager
	virtual BufferManager* getBufferManager() const;

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
};

} } // end namespaces