/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#ifdef VAPOR_RENDERER_OPENGL

#include "Core/Reflection.h"
#include "Math/Color.h"
#include "Math/Vector.h"
#include "Math/Matrix4x3.h"

#include "Render/Render.h"
#include "Render/Window.h"
#include "Render/Target.h"
#include "Render/Renderable.h"
#include "Render/RenderQueue.h"

NAMESPACE_ENGINE_BEGIN

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

typedef std::map<Light*, Texture*> ShadowTextureMap;

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

class API_RENDER RenderDevice
{
	DECLARE_UNCOPYABLE(RenderDevice)

public:

	RenderDevice();
	~RenderDevice();

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

	// Gets/sets the current active render target.
	GETTER(RenderTarget, RenderTarget*, activeTarget)
	void setRenderTarget(RenderTarget* target);

	// Gets the current rendering pipeline.
	ACESSOR(Pipeline, RenderPipeline::Enum, pipeline)

	// Gets the texture manager.
	GETTER(ShadowTextures, ShadowTextureMap, shadowTextures)

	// Gets the active context.
	GETTER(ActiveContext, RenderContext*, activeContext)

	// Gets the active view.
	GETTER(ActiveView, RenderView*, activeView)

	// Returns true if device is using fixed pipeline.
	bool isFixedPipeline() const;

protected:

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
	void setupRenderStateMaterial(const RenderState& state, bool bindUniforms );
	void undoRenderStateMaterial( Material* );

	void unbindTextures(Material* material);
	void bindTextures(const RenderState& state, bool bindUniforms);

	// Rendering pipeline.
	RenderPipeline::Enum pipeline;

	// Active render target
	RenderTarget* activeTarget;

	// Active render context.
	RenderContext* activeContext;

	// Active view.
	RenderView* activeView;

	ShadowTextureMap shadowTextures;
	RenderBuffer* shadowDepthBuffer;
	void updateLightDepth( LightState& state );
};

API_RENDER RenderDevice* GetRenderDevice();

//-----------------------------------//

NAMESPACE_ENGINE_END

#endif