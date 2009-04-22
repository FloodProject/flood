/************************************************************************
*
* vaporEngine by triton © (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Engine.h"

#include "vapor/render/Window.h"
#include "vapor/render/RenderQueue.h"
#include "vapor/render/Target.h"
#include "vapor/render/Adapter.h"
#include "vapor/render/BufferManager.h"
#include "vapor/render/VertexBuffer.h"

#include "vapor/math/Matrix.h"

using namespace vapor::math;

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

	//-----------------------------------//

	/**
	 * Creates a new rendering device, using the rendering API specified.
	 * TODO: add suport for passing a specific backend
	 */
	static Device& createDevice( Settings& settings = Settings(640, 480) );

	/**
	 * Clears the active render target.
	 */
	virtual void clearTarget( ) = 0;

	/**
	 * Updates the target render target.
	 */ 
	virtual void updateTarget( );

	/**
	 * Sets the active render target.
	 */
	virtual void setRenderTarget( RenderTarget *renderTarget );

	//-----------------------------------//

	// create a window if not passed a WindowHandle
	virtual void open( Settings& wS ) = 0;

	// get window class
	virtual Window& getWindow();

	// get adapter information
	virtual Adapter& getAdapter();

	//-----------------------------------//

	// draw a vertex buffer
	//virtual void draw(unsigned long Mode, unsigned long First, unsigned long Count);

	//-----------------------------------//

	/**
	 * Gets the buffer manager.
	 */
	virtual BufferManager& getBufferManager();

protected:

	// active render target
	RenderTarget* _activeTarget;

	// list of render targets
	//vector<RenderTarget*> _renderTargets;

	// adapter information
	Adapter* _adapter;

	// render window
	Window* _window;

	// manages all buffers
	BufferManager* _bufferManager;
};

} } // end namespaces

//-----------------------------------//

// get projection matrix
//virtual const Matrix4x3 & getProjectionMatrix () const = 0;

// set projection matrix
//virtual void setProjectionMatrix (const Matrix4x3 &proj) = 0;

// push projection matrix on stack
//virtual void pushProjectionMatrix (const Matrix4x3 &proj) = 0;

// pop projection matrix from stack
//virtual void popProjectionMatrix (bool set = true) = 0;
