/************************************************************************
*
* Flood Project � (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Core/Reflection.h"
#include "Core/Math/Color.h"
#include "Core/Math/Vector.h"
#include "Core/Math/Matrix4x3.h"

#include "Graphics/RenderTarget.h"
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

API_GRAPHICS REFLECT_DECLARE_ENUM(RenderPipeline)

enum struct RenderPipeline
{
	Fixed,
	ShaderForward,
	ShaderDeferred
};

//-----------------------------------//

class RenderBuffer;
class RenderContext;
class RenderBackend;
class RenderBatch;

/**
 * Represents the rendering device we are using. At startup the application
 * can ask the user what device he wants to use (OpenGL / DirectX / Software)
 * and then we create and use the device. All operations are to be done by this
 * abstract class. The user of the engine doesn't need to care about internal
 * details how all the rendering operations are implemented.
 *
 * This class only knows about rendering primitives (Vertex/Index buffers) and
 * renderables. It does not know anything about high-level objects, like meshes.
 *
 * Each render device will also manage a list of render targets (windows, FBOs).
 * Window implementations depend on the rendering system used, for example, an
 * OpenGL window will be different from an DirectX window, so this is also
 * responsible for creating a new window if no window handle is passed to it.
 */

typedef HashMap<Light*, Texture*> ShadowTextureMap;

class API_GRAPHICS RenderDevice
{
	DECLARE_UNCOPYABLE(RenderDevice)

public:

	RenderDevice();
	~RenderDevice();

	// Renders a renderable.
	void render( const RenderState& state, const LightQueue& lights );

	// Renders a list of renderables.
	void render( RenderBlock& queue );

	// Gets the current rendering pipeline.
	ACCESSOR(Pipeline, RenderPipeline, pipeline)

	// Gets the current rendering backend.
	ACCESSOR(Backend, RenderBackend*, renderBackend)

	// Renders and updates into all render targets.
	void updateRenderTargets();

	// Gets/sets the current active render target.
	GETTER(RenderTarget, RenderTarget*, activeTarget)
	void setRenderTarget(RenderTarget* target);

	// Gets/sets the active context.
	ACCESSOR(ActiveContext, RenderContext*, activeContext)

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

	void bindTextureUnits(const RenderState& state, bool bindUniforms);
	void unbindTextureUnits(Material* material);

	// Binds the buffers needed to draw the batch.
	bool bindBuffers(RenderBatch*);
	bool unbindBuffers(RenderBatch*);

	// Rendering pipeline.
	RenderPipeline pipeline;

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

API_GRAPHICS RenderDevice* GetRenderDevice();

//-----------------------------------//

NAMESPACE_GRAPHICS_END
