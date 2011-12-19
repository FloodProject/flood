/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#ifdef ENABLE_RENDERER_OPENGL

#include "Core/Reflection.h"
#include "Math/Color.h"
#include "Math/Vector.h"
#include "Math/Matrix4x3.h"

#include "Graphics/Render.h"
#include "Graphics/RenderTarget.h"
#include "Graphics/RenderBatch.h"
#include "Graphics/RenderQueue.h"

NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------//

class Light;
class Texture;

/**
 * There are a couple rendering pipelines. You can choose each one depending
 * on the hardware functionality (shaders don't exist on some old or mobile
 * hardware) or user preference (fixed function is faster on some machines).
 */

REFLECT_DECLARE_ENUM(RenderPipeline)

struct RenderPipeline
{
	enum Enum
	{
		Fixed,
		ShaderForward,
		ShaderDeferred
	};
};

//-----------------------------------//

class RenderBuffer;
class RenderContext;
class RenderBackend;

/**
 * Represents the rendering device we are using. At startup the application
 * can ask the user what device he wants to use (OpenGL / DirectX / Software)
 * and then we create and use the device. All operations are to be done by this
 * abstract class. The user of the engine doesn't need to care about internal
 * details how all the rendering operations are implemented.
 *
 * This class only knows about rendering primitives (Vertex/Index buffers) and
 *.renderables. It does not know anything about high-level objects, like meshes.
 *
 * Each render device will also manage a list of render targets (windows, FBOs).
 * Window implementations depend on the rendering system used, for example, an
 * OpenGL window will be different from an DirectX window, so this is also
 * responsible for creating a new window if no window handle is passed to it.
 */

typedef std::map<Light*, Texture*> ShadowTextureMap;

class API_RENDER RenderDevice
{
	DECLARE_UNCOPYABLE(RenderDevice)

public:

	RenderDevice();
	~RenderDevice();

	// Renders a renderable.
	void render( const RenderState& state, const LightQueue& lights );

	// Renders a list of.renderables.
	void render( RenderBlock& queue );

	// Gets the current rendering pipeline.
	ACESSOR(Pipeline, RenderPipeline::Enum, pipeline)

	// Gets the texture manager.
	//GETTER(ShadowTextures, ShadowTextureMap, shadowTextures)

	// Renders and updates into all render targets.
	void updateRenderTargets();

	// Gets/sets the current active render target.
	GETTER(RenderTarget, RenderTarget*, activeTarget)
	void setRenderTarget(RenderTarget* target);

	// Gets/sets the active context.
	ACESSOR(ActiveContext, RenderContext*, activeContext)

	// Gets the active view.
	GETTER(ActiveView, RenderView*, activeView)
	
	// Sets the active view.
	void setActiveView( RenderView* view );

	// Clears the active render view.
	void clearView();

	// Returns true if device is using fixed pipeline.
	bool isFixedPipeline() const;

protected:

	// Forward render state management.
	bool setupRenderStateMatrix( const RenderState& state );
	bool setupRenderStateShadow( LightQueue& lights );
	//bool setupRenderStateLight( const RenderState&, const LightQueue& );
	bool setupRenderStateOverlay( const RenderState& );

	// Binds the buffers needed to draw the batch.
	bool bindBuffers(RenderBatch*);
	bool unbindBuffers(RenderBatch*);

	// Rendering pipeline.
	RenderPipeline::Enum pipeline;

	// Active render target
	RenderTarget* activeTarget;

	// Active render context.
	RenderContext* activeContext;

	// Active render backend.
	RenderBackend* renderBackend;

	// Active view.
	RenderView* activeView;

#if 0
	ShadowTextureMap shadowTextures;
	RenderBuffer* shadowDepthBuffer;
	void updateLightDepth( LightState& state );
#endif
};

API_RENDER RenderDevice* GetRenderDevice();

//-----------------------------------//

NAMESPACE_GRAPHICS_END

#endif