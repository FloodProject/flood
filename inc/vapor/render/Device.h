/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#ifdef VAPOR_RENDERER_OPENGL

#include "Render/Window.h"
#include "Render/Target.h"
#include "Render/Renderable.h"
#include "Render/RenderQueue.h"
#include "Math/Color.h"
#include "Math/Vector2.h"
#include "Math/Matrix4x3.h"
#include "Reflection.h"

namespace vapor {

//-----------------------------------//

class Camera;
class Adapter;
class RenderBuffer;
class BufferManager;
class ResourceManager;
class ProgramManager;
class TextureManager;

/**
 * There are a couple rendering pipelines. You can choose each one depending
 * on the hardware functionality (shaders don't exist on some old or mobile
 * hardware) or user preference (fixed function is faster on some machines).
 */

struct RenderPipeline
{
	DECLARE_ENUM()

	enum Enum
	{
		Fixed,
		ShaderForward,
		ShaderDeferred
	};
};

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

class VAPOR_API RenderDevice
{
	DECLARE_UNCOPYABLE(RenderDevice)

public:

	RenderDevice();
	~RenderDevice();

	// Initializes the rendering system (needs an OpenGL context).
	void init();

	// Renders a renderable.
	void render( const RenderState& state, const LightQueue& lights );

	// Renders a list of renderables.
	void render( RenderBlock& queue );

	// Sets the active view.
	void setView( RenderView* view );

	// Clears the active render target.
	void clearView();

	// Renders and updates into all render targets.
	void updateRenderTargets();

	// Creates a new render buffer (offscreen render target).
	RenderBuffer* createRenderBuffer( const Settings& );

	// Creates a new rendering window.
	Window* createWindow( const WindowSettings& = WindowSettings() );

	// Gets the main render window.
	GETTER(RenderWindow, Window*, window)

	// Gets/sets the current active render target.
	GETTER(RenderTarget, RenderTarget*, activeTarget)
	void setRenderTarget(RenderTarget* target);

	// Gets the framebuffer color in the given pixel location.
	// Note that point (0,0) is located in the top left corner.
	Color getPixel(ushort x, ushort y) const;

	// Gets/sets the main rendering window.
	ACESSOR(Window, Window*, window)

	// Sets the current clear color.
	void setClearColor(const Color& color);

	// Gets the current rendering pipeline.
	ACESSOR(Pipeline, RenderPipeline::Enum, pipeline)

	// Gets rendering adapter information.
	GETTER(Adapter, Adapter*, adapter)

	// Gets the program manager.
	GETTER(ProgramManager, ProgramManager*, programManager)

	// Gets the texture manager.
	GETTER(TextureManager, TextureManager*, textureManager)

	// Gets the texture manager.
	GETTER(ShadowTextures, ShadowTextureMap, shadowTextures)

	// Gets the active view.
	GETTER(ActiveView, RenderView*, activeView)

protected:

	// Checks that all needed OpenGL extensions are available.
	void checkExtensions();

	// Fixed render state management.
	void setupRenderFixed( const RenderState&, const LightQueue& );
	bool setupRenderFixedMatrix( const RenderState& state );
	bool setupRenderFixedOverlay( const RenderState& state );
	//bool setupRenderFixedShadow( LightQueue& lights );
	//bool setupRenderFixedLight( const RenderState&, const LightQueue& );
	//bool setupRenderFixedOverlay( const RenderState& );

	// Forward render state management.
	void setupRenderForward( const RenderState&, const LightQueue& );
	bool setupRenderStateMatrix( const RenderState& state );
	bool setupRenderStateShadow( LightQueue& lights );
	bool setupRenderStateLight( const RenderState&, const LightQueue& );
	bool setupRenderStateOverlay( const RenderState& );

	// Common render state management.
	void setupRenderStateMaterial( const MaterialPtr& );
	void undoRenderStateMaterial( const MaterialPtr& );

	// Rendering pipeline.
	RenderPipeline::Enum pipeline;

	// Manages all the shaders.
	TextureManager* textureManager;

	// Manages all the shaders.
	ProgramManager* programManager;

	// Active render target
	RenderTarget* activeTarget;

	// List of render targets
	std::vector<RenderTarget*> renderTargets;

	// Active view.
	RenderView* activeView;

	// Adapter information
	Adapter* adapter;

	// Render window
	Window* window;

	ShadowTextureMap shadowTextures;
	RenderBuffer* shadowDepthBuffer;

	void updateLightDepth( LightState& state );
};

//-----------------------------------//

} // end namespace

#endif