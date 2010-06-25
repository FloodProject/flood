/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#ifdef VAPOR_RENDERER_OPENGL

#include "vapor/math/Color.h"
#include "vapor/math/Vector2.h"
#include "vapor/math/Matrix4x3.h"

#include "vapor/render/Window.h"
#include "vapor/render/Target.h"
#include "vapor/render/Renderable.h"
#include "vapor/render/RenderQueue.h"

namespace vapor {

//-----------------------------------//

class Camera;
class Adapter;
class RenderBuffer;
class BufferManager;
class ResourceManager;
class ProgramManager;
class TextureManager;

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

typedef std::map<LightPtr, TexturePtr> ShadowTextureMap;

class VAPOR_API RenderDevice : private boost::noncopyable
{
public:

	RenderDevice( ResourceManager* );
	~RenderDevice();

	// Initializes the rendering system.
	// Note: Needs an active OpenGL context.
	void init();

	// Renders a list of renderables.
	void render( RenderBlock& queue, const Camera* cam );

	// Gets the framebuffer color in the given pixel location.
	// Note: it follows OpenGL 2D axis conventions, so the point (0,0)
	// is located in the top left corner of the screen.
	Color getPixel(ushort x, ushort y) const;

	// Renders and updates into all render targets.
	void updateRenderTargets();

	// Clears the active render target.
	void clearTarget();

	// Sets the window as the active rendering target.
	void setWindowActiveTarget();

	// Creates a new render buffer (offscreen render target).
	RenderBuffer* createRenderBuffer( const Settings& );

	// Creates a new rendering window.
	Window* createWindow( const WindowSettings& = WindowSettings() );

	// Gets the main render window.
	GETTER(RenderWindow, Window*, window)

	// Gets/sets the current active render target.
	GETTER(RenderTarget, RenderTarget*, activeTarget)
	void setRenderTarget(RenderTarget* target);

	// Gets/sets the main rendering window.
	ACESSOR(Window, Window*, window)

	// Gets/sets the current clear color.
	GETTER(ClearColor, const Color&, color)
	void setClearColor(const Color& color);

	// Sets the OpenGL viewport dimensions.
	void setViewport( const Vector2i&, const Vector2i& );

	// Gets rendering adapter information.
	GETTER(Adapter, Adapter*, adapter)

	// Gets the program manager.
	GETTER(ProgramManager, ProgramManager*, programManager)

	// Gets the texture manager.
	GETTER(TextureManager, TextureManager*, textureManager)

	// Gets the texture manager.
	GETTER(ShadowTextures, ShadowTextureMap, shadowTextures)

protected:

	// Checks that all needed OpenGL extensions are available.
	void checkExtensions();

	// Render state management.
	bool setupRenderState( const RenderState&, const Camera* );
	bool setupRenderStateShadow( LightQueue& lights );
	bool setupRenderStateLight( const RenderState&, const LightQueue& );
	bool setupRenderStateOverlay( const RenderState& );
	void setupRenderStateMaterial( const MaterialPtr& );
	void undoRenderStateMaterial( const MaterialPtr& );

	ResourceManager* resourceManager;

	// Manages all the shaders.
	TextureManager* textureManager;

	// Manages all the shaders.
	ProgramManager* programManager;

	// Active render target
	RenderTarget* activeTarget;

	// List of render targets
	std::vector<RenderTarget*> renderTargets;

	// Adapter information
	Adapter* adapter;

	// Render window
	Window* window;

	// Current clear color
	Color color;

	// Current viewport dimensions.
	Vector2i viewportLeft, viewportSize;

	ShadowTextureMap shadowTextures;
	RenderBuffer* shadowDepthBuffer;
	const Camera* camera;

	void updateLightDepth( LightState& state );
};

//-----------------------------------//

} // end namespace

#endif