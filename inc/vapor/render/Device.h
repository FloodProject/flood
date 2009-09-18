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
#include "vapor/render/Target.h"
#include "vapor/render/Adapter.h"

#include "vapor/render/Renderable.h"

#include "vapor/math/Matrix.h"
#include "vapor/math/Color.h"

namespace vapor {
	namespace render {

//-----------------------------------//

class BufferManager;

//-----------------------------------//

/**
 * Represents the rendering device we are using. At startup the application
 * can ask the user what device he wants to use (OpenGL / DirectX / Software)
 * and then we create and use the device. All operations are to be done by this
 * abstract class. The user of the engine doesn't need to care about internal
 * details how all the rendering operations are implemented.
 *
 * This class only knows about rendering primitives (Vertex/Index buffers) and
 * Renderables. It does not know anything about high-level objects, like meshes.
 *
 * Each render device will also manage a list of render targets (Windows or RTT).
 * Window implementations depend on the rendering system used, for example, an
 * OpenGL window will be different from an DirectX window, so this is also
 * responsible for creating a new window if no window handle is passed to it.
 */

class Device : private boost::noncopyable
{
public:

	Device();
	~Device();

	// Initializes the rendering system. (Needs an an OpenGL context)
	void init();

	// Creates a new render window.
	Window& createWindow( const WindowSettings& settings = WindowSettings() );

	// Gets the main window.
	Window* getWindow() const;

	// Gets rendering adapter information.
	Adapter* getAdapter() const;

	// Gets the buffer manager.
	BufferManager* getBufferManager() const;

	// Gets the current active render target.
	render::Target* getRenderTarget() const;

	// Renders a list of renderables
	void render( RenderQueue& queue );

	// Updates the target render target.
	void updateTarget( );

	// Sets the current clear color.
	void setClearColor(math::Color c);

	// Sets the active render target.
	void setRenderTarget( RenderTarget* renderTarget );

	// Clears the active render target.
	void clearTarget();

protected:

	// Active render target
	RenderTarget* activeTarget;

	// List of render targets
	std::vector< RenderTarget* > renderTargets;

	// Adapter information
	Adapter* adapter;

	// Render window
	Window* window;

	// Manages all buffers
	BufferManager* bufferManager;

	// Current clear color
	math::Color clearColor;

private:

	void checkExtensions();
};

//-----------------------------------//

typedef tr1::shared_ptr< Device > DevicePtr;

//-----------------------------------//

} } // end namespaces

#endif