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
#include "vapor/render/Renderable.h"
#include "vapor/render/RenderQueue.h"
#include "vapor/math/Matrix4x3.h"
#include "vapor/math/Vector2.h"
#include "vapor/math/Color.h"

FWD_DECL(scene, Camera);
FWD_DECL(render, BufferManager);
FWD_DECL_NS_TYPEDEF_PTR(render, ProgramManager)
FWD_DECL_NS_TYPEDEF_PTR(render, TextureManager)
FWD_DECL_NS_TYPEDEF_PTR(render, Adapter)
FWD_DECL_NS_TYPEDEF_PTR(render, RenderBuffer)

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
 * Each render device will also manage a list of render targets (Windows, FBOs).
 * Window implementations depend on the rendering system used, for example, an
 * OpenGL window will be different from an DirectX window, so this is also
 * responsible for creating a new window if no window handle is passed to it.
 */

typedef std::map<scene::LightPtr, TexturePtr> ShadowTextureMap;

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

	// Renders and updates into all render targets.
	void updateRenderTargets();

	// Clears the active render target.
	void clearTarget();

	// Sets the window as the active rendering target.
	void setWindowActiveTarget();

	// Creates a new render buffer (offscreen render target).
	RenderBufferPtr createRenderBuffer( const Settings& );

	// Creates a new rendering window.
	WindowPtr createWindow( const WindowSettings& = WindowSettings() );

	// Gets the main render window.
	IMPLEMENT_GETTER(RenderWindow, WindowPtr, window)

	// Gets/sets the current active render target.
	IMPLEMENT_GETTER(RenderTarget, RenderTargetPtr, activeTarget)
	void setRenderTarget(RenderTargetPtr target);

	// Gets/sets the main rendering window.
	IMPLEMENT_ACESSOR(Window, WindowPtr, window)

	// Gets/sets the current clear color.
	IMPLEMENT_GETTER(ClearColor, const math::Color&, color)
	void setClearColor(const math::Color& color);

	// Sets the OpenGL viewport dimensions.
	void setViewport( const math::Vector2i&, const math::Vector2i& );

	// Gets rendering adapter information.
	IMPLEMENT_GETTER(Adapter, AdapterPtr, adapter)

	// Gets the program manager.
	IMPLEMENT_GETTER(ProgramManager, ProgramManagerPtr, programManager)

	// Gets the texture manager.
	IMPLEMENT_GETTER(TextureManager, TextureManagerPtr, textureManager)

	// Gets the texture manager.
	IMPLEMENT_GETTER(ShadowTextures, ShadowTextureMap, shadowTextures)

protected:

	// Checks that all needed OpenGL extensions are available.
	void checkExtensions();

	// Render state management.
	bool setupRenderState( const RenderState&, const scene::Camera* );
	bool setupRenderStateShadow( LightQueue& lights );
	bool setupRenderStateLight( const RenderState&, const LightQueue& );
	bool setupRenderStateOverlay( const RenderState& );
	void setupRenderStateMaterial( const MaterialPtr& );
	void undoRenderStateMaterial( const MaterialPtr& );

	// Manages all the shaders.
	TextureManagerPtr textureManager;

	// Manages all the shaders.
	ProgramManagerPtr programManager;

	// Active render target
	RenderTargetPtr activeTarget;

	// List of render targets
	std::vector<RenderTargetPtr> renderTargets;

	// Adapter information
	AdapterPtr adapter;

	// Render window
	WindowPtr window;

	// Current clear color
	math::Color color;

	// Current viewport dimensions.
	math::Vector2i viewportLeft, viewportSize;

	ShadowTextureMap shadowTextures;
	RenderBufferPtr shadowDepthBuffer;
	const scene::Camera* camera;

	void updateLightDepth( LightState& state );
};

TYPEDEF_PTR(Device)

//-----------------------------------//

} } // end namespaces

#endif