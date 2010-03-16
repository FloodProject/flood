/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#ifdef VAPOR_RENDERER_OPENGL

#include "vapor/render/Window.h"
#include "vapor/render/Target.h"
#include "vapor/render/Adapter.h"
#include "vapor/render/TextureManager.h"
#include "vapor/render/ProgramManager.h"
#include "vapor/render/Renderable.h"
#include "vapor/render/RenderQueue.h"
#include "vapor/math/Matrix4x3.h"
#include "vapor/math/Color.h"

FWD_DECL(scene, Camera);
FWD_DECL(render, BufferManager);

namespace vapor { namespace render { 

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

class VAPOR_API Device : private boost::noncopyable
{
public:

	Device();
	~Device();

	// Initializes the rendering system.
	// Note: Needs an active OpenGL context.
	void init();

	// Renders a list of renderables.
	void render( RenderBlock& queue, const scene::Camera* cam );

	// Updates the render target.
	void updateTarget();

	// Clears the active render target.
	void clearTarget();

	// Sets the window as the active rendering target.
	void setWindowActiveTarget();

	// Gets/sets the current active render target.
	IMPLEMENT_ACESSOR_PTR(RenderTarget, RenderTarget&, activeTarget)

	// Gets/sets the main rendering window.
	IMPLEMENT_ACESSOR_PTR(Window, Window&, window)

	// Gets rendering adapter information.
	IMPLEMENT_GETTER(Adapter, Adapter&, *adapter)

	// Sets the current clear color.
	IMPLEMENT_SETTER(ClearColor, const math::Color&, clearColor)

	// Gets the texture manager.
	IMPLEMENT_GETTER(TextureManager, TextureManager&, TextureManager::getInstance())

	// Creates a new rendering window.
	Window& createWindow( const WindowSettings& settings = WindowSettings() );

protected:

	// Active render target
	RenderTarget* activeTarget;

	// List of render targets
	std::vector< RenderTarget* > renderTargets;

	// Adapter information
	Adapter* adapter;

	// Render window
	Window* window;

	// Current clear color
	math::Color clearColor;

private:

	void checkExtensions();
};

//-----------------------------------//

} } // end namespaces

#endif