/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"

#ifdef VAPOR_RENDERER_OPENGL

#include "Core/Utilities.h"
#include "Render/Device.h"
#include "Render/Render.h"
#include "Render/RenderContext.h"
#include "Render/GL.h"
#include "Render/View.h"
#include "Render/ProgramManager.h"
#include "Render/TextureManager.h"

#include "Scene/Entity.h"
#include "Scene/Transform.h"
#include "Scene/Camera.h"
#include "Scene/Light.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

REFLECT_ENUM(RenderPipeline)
	ENUM(Fixed)
	ENUM(ShaderForward)
	ENUM(ShaderDeferred)
REFLECT_ENUM_END()

//-----------------------------------//

static RenderDevice* gs_RenderDevice = nullptr;
RenderDevice* GetRenderDevice() { return gs_RenderDevice; }

static Allocator* gs_RenderAllocator = nullptr;
Allocator* GetRenderAllocator() { return gs_RenderAllocator; }

static Allocator* gs_FrameAllocator = nullptr;
Allocator* GetFrameAllocator() { return gs_FrameAllocator; }

#define MEGABYTE (1 << 20)

void RenderInitialize()
{
	gs_RenderAllocator = AllocatorCreateHeap(AllocatorGetHeap());
	AllocatorSetGroup(gs_RenderAllocator, "Render");

	gs_FrameAllocator = AllocatorCreateBump(gs_RenderAllocator, 1*MEGABYTE);
	AllocatorSetGroup(gs_FrameAllocator, "Frame");
}

void RenderDeinitialize()
{
	AllocatorDestroy(gs_FrameAllocator);
	AllocatorDestroy(gs_RenderAllocator);
};


//-----------------------------------//

RenderDevice::RenderDevice()
	: activeContext(nullptr)
	, activeTarget(nullptr)
	, activeView(nullptr)
	, shadowDepthBuffer(nullptr)
	, pipeline(RenderPipeline::ShaderForward)
{
	gs_RenderDevice = this;
}

//-----------------------------------//

RenderDevice::~RenderDevice()
{
}

//-----------------------------------//

static bool RenderStateSorter(const RenderState& lhs, const RenderState& rhs)
{
	int rA = lhs.renderable->getRenderLayer();
	int rB = rhs.renderable->getRenderLayer();
	int pA = lhs.priority;
	int pB = rhs.priority;

	return (rA == rB) ? (pA < pB) : (rA < rB);
}

void RenderDevice::render( RenderBlock& queue ) 
{
	//setupRenderStateShadow( queue.lights );

	// Sort the renderables by render group (TODO: use a radix sorter).
	std::sort( queue.renderables.begin(), queue.renderables.end(), &RenderStateSorter );

	// Render all the renderables in the queue.
	for( size_t i = 0; i < queue.renderables.size(); i++ )
	{
		const RenderState& state = queue.renderables[i];
		render(state, queue.lights);
	}
}

//-----------------------------------//

void RenderDevice::render( const RenderState& state, const LightQueue& lights )
{
#ifdef DEBUG_BUILD
	const RenderablePtr& renderable = state.renderable;
	if( !renderable ) return;

	const Material* material = state.material;
	if( !material ) return;
#endif

	switch(pipeline)
	{
	case RenderPipeline::Fixed:
		setupRenderFixed(state, lights);
		return;
	case RenderPipeline::ShaderForward:
		setupRenderForward(state, lights);
		return;
	default:
		LogError("Unknown render pipeline");
		return;
	}
}

//-----------------------------------//

void RenderDevice::setupRenderFixed(const RenderState& state, const LightQueue& lights)
{
	const RenderablePtr& renderable = state.renderable;
	renderable->bind();

	const VertexBufferPtr& vb = renderable->getVertexBuffer();
	vb->bindPointers();
	
	Material* material = state.material;
	setupRenderStateMaterial(state, false);

	RenderLayer::Enum stage = renderable->getRenderLayer();

	if( stage != RenderLayer::Overlays )
	{
		if( !setupRenderFixedMatrix(state) )
			return;
	}
	else if( stage == RenderLayer::Overlays )
	{
		if( !setupRenderFixedOverlay(state) )
			return;
	}

	glDisable(GL_LIGHTING);

	glPushMatrix();

	const Matrix4x4& model = state.modelMatrix;
	glMultMatrixf(&model.m11);

	renderable->onPreRender(state);
	renderable->render(this);
	renderable->onPostRender(state);

	glPopMatrix();

	undoRenderStateMaterial(material);

	vb->unbindPointers();	
	renderable->unbind();
}

//-----------------------------------//

bool RenderDevice::setupRenderFixedMatrix( const RenderState& state )
{
	CameraPtr camera = activeView->getCamera();
	if( !camera ) return false;

	const Frustum& frustum = camera->getFrustum();

	const Matrix4x4& view = Matrix4x4(camera->getViewMatrix());
	const Matrix4x4& projection = frustum.matProjection;

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(&projection.m11);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(&view.m11);

	return true;
}

//-----------------------------------//

bool RenderDevice::setupRenderFixedOverlay( const RenderState& state )
{
	Vector2i size = activeTarget->getSettings().getSize();
	
	Matrix4x4 projection = Matrix4x4::createOrthographic(0, size.x, size.y, 0, 0, 100);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(&projection.m11);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.375f, 0.375f, 0.0f);

	return true;
}

//-----------------------------------//

void RenderDevice::setupRenderForward(const RenderState& state, const LightQueue& lights)
{
	const RenderablePtr& renderable = state.renderable;
	renderable->bind();

	const VertexBufferPtr& vb = renderable->getVertexBuffer();
	vb->bindGenericPointers();

	Material* material = state.material;
	
	const String& name = material->getProgram();
	const ProgramPtr& program = activeContext->programManager->getProgram(name);
	if( !program ) return;

	if( !program->isLinked() )
	{
		program->link();
	}

	program->bind();

	setupRenderStateMaterial(state, true);

	RenderLayer::Enum stage = renderable->getRenderLayer();

	if( stage != RenderLayer::Overlays )
	{
		if( !setupRenderStateMatrix(state) )
			return;

		if( !setupRenderStateLight(state, lights) )
			return;
	}
	else if( stage == RenderLayer::Overlays )
	{
		if( !setupRenderStateOverlay(state) )
			return;
	}

	if( !renderable->onPreRender.empty() )
		renderable->onPreRender(state);
	
	const UniformBufferPtr& ub = renderable->getUniformBuffer();
	program->setUniforms(ub);

	renderable->render(this);
	
	if( !renderable->onPostRender.empty() )
		renderable->onPostRender(state);
	
	undoRenderStateMaterial(material);
	
	renderable->unbind();
	program->unbind();
	vb->unbindGenericPointers();
}

//-----------------------------------//

bool RenderDevice::setupRenderStateMatrix( const RenderState& state )
{
	CameraPtr camera = activeView->getCamera();
	if( !camera ) return false;

	const Frustum& frustum = camera->getFrustum();

	const Matrix4x3& matModel = state.modelMatrix;
	const Matrix4x3& matView = camera->getViewMatrix();
	const Matrix4x4& matProjection = frustum.matProjection;

	const UniformBufferPtr& ub = state.renderable->getUniformBuffer();
	ub->setUniform( "vp_ModelMatrix", matModel );
	ub->setUniform( "vp_ViewMatrix", matView );
	ub->setUniform( "vp_ProjectionMatrix", matProjection );

	return true;
}

//-----------------------------------//

void RenderDevice::bindTextures(const RenderState& state, bool bindUniforms)
{
	TextureUnitMap& units = state.material->textureUnits;
	const UniformBufferPtr& ub = state.renderable->getUniformBuffer();

	TextureUnitMap::const_iterator it;
	for( it = units.begin(); it != units.end(); it++ )
	{
		int32 index = it->first;
		const ImageHandle& image = it->second;

		const TexturePtr& texture = activeContext->textureManager->getTexture(image.Resolve());
		texture->bind(index);

		if( !bindUniforms ) continue;

		String uniform = "vp_Texture" + StringFromNumber(index);
		ub->setUniform( uniform, index );
	}
}

//-----------------------------------//

void RenderDevice::unbindTextures(Material* material)
{
	TextureUnitMap& units = material->textureUnits;

	TextureUnitMap::const_iterator it;
	for( it = units.begin(); it != units.end(); it++ )
	{
		const ImageHandle& handle = it->second;
		const TexturePtr& tex = activeContext->textureManager->getTexture(handle.Resolve());
		tex->unbind( it->first );
	}
}

//-----------------------------------//

void RenderDevice::updateLightDepth( LightState& state )
{
#if 0
	const LightPtr& light = state.light;
	assert( light->getLightType() == LightType::Directional );

	TexturePtr shadowDepthTexture;
	
	if( !shadowDepthBuffer )
	{
		shadowDepthBuffer = createRenderBuffer( Settings(512, 512) );
	}

	if( shadowTextures.find(light) == shadowTextures.end() )
	{
		shadowDepthTexture = shadowDepthBuffer->createRenderTexture();
		shadowTextures[light] = shadowDepthTexture;
	}
	else
	{
		shadowDepthTexture = shadowTextures[light];
	}

	CameraPtr lightCamera( new Camera(*camera) );
	TransformPtr lightTransform( new Transform(*state.transform.get()) );
	
	EntityPtr lightCameraEntity( new Entity("ShadowCamera") );
	lightCameraEntity->addTransform(); /*Component( lightTransform );*/
	lightCameraEntity->addComponent( lightCamera );

	RenderView* lightView = new View(lightCamera, shadowDepthBuffer);

	if( !shadowDepthBuffer->check() )
		return;

	// TODO: turn off color writes (glColorMask?)
	lightView->update();
	shadowDepthBuffer->unbind();

	Matrix4x4 bias;
	bias.identity();
	bias.m11 = 0.5f;
	bias.m22 = 0.5f;
	bias.m33 = 0.5f;
	bias.tx  = 0.5f;
	bias.ty  = 0.5f;
	bias.tz  = 0.5f;

	const Frustum& frustum = lightCamera->getFrustum();
	const Matrix4x4& matProjection = frustum.projectionMatrix;

	state.projection = lightCamera->getViewMatrix()
		* matProjection * bias;
#endif
}

//-----------------------------------//

bool RenderDevice::setupRenderStateShadow( LightQueue& lights )
{
	if( lights.empty() ) 
		return true;

	for( size_t i = 0; i < lights.size(); i++ )
	{
		LightState& state = lights[i];
		const Light* light = state.light;

		if( !light->getCastsShadows() )
			continue;

		updateLightDepth( state );
	}

	return true;
}

//-----------------------------------//

bool RenderDevice::setupRenderStateLight( const RenderState& state, const LightQueue& lights )
{
	const UniformBufferPtr& ub = state.renderable->getUniformBuffer();

	for( size_t i = 0; i < lights.size(); i++ )
	{
		const LightState& lightState = lights[i];
		const Light* light = lightState.light;

		//TexturePtr shadowDepthTexture;
		//shadowDepthTexture = shadowTextures[light];
		//assert( shadowDepthTexture != nullptr );

		std::vector< Color > colors;
		colors.push_back( light->getDiffuseColor() );
		colors.push_back( light->getSpecularColor() );
		colors.push_back( light->getEmissiveColor() );
		colors.push_back( light->getAmbientColor() );

		const Transform* transform = lightState.transform;

		// TODO: fix the lighting stuff
		ub->setUniform( "vp_LightColors", colors );
		ub->setUniform( "vp_LightDirection", transform->getRotationMatrix() );
		//ub->setUniform( "vp_ShadowMap", shadowDepthTexture->id() );
		//ub->setUniform( "vp_CameraProj", state.modelMatrix * lightState.projection );
	}

	return true;
}

//-----------------------------------//

bool RenderDevice::setupRenderStateOverlay( const RenderState& state )
{
	Vector2i size = activeTarget->getSettings().getSize();
	Matrix4x4 projection = Matrix4x4::createOrthographic(0, size.x, size.y, 0, 0, 100);

	const UniformBufferPtr& ub = state.renderable->getUniformBuffer();
	ub->setUniform( "vp_ProjectionMatrix", projection );
	ub->setUniform( "vp_ModelMatrix", state.modelMatrix );
	ub->setUniform( "vp_ViewMatrix", Matrix4x4::Identity );
	ub->setUniform( "vp_ModelViewMatrix", state.modelMatrix );

	return true;
}

//-----------------------------------//

void RenderDevice::setupRenderStateMaterial( const RenderState& state, bool bindUniforms )
{
	bindTextures(state, bindUniforms);
	Material* mat = state.material;

	if( mat->lineSmooth )
	{
		glEnable( GL_LINE_SMOOTH );
		glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
	}
	
	if( mat->lineWidth != Material::DefaultLineWidth )
		glLineWidth( mat->getLineWidth() );

	if( mat->depthCompare != DepthCompare::Less )
		glDepthFunc( mat->depthCompare );

	if( !mat->cullBackfaces )
		glDisable( GL_CULL_FACE );

	if( mat->alphaTest )
		glEnable( GL_ALPHA_TEST );

	if( !mat->depthTest )
		glDisable( GL_DEPTH_TEST );

	if( !mat->depthWrite )
		glDepthMask( GL_FALSE );

	if( mat->isBlendingEnabled() ) 
	{
		glEnable( GL_BLEND );
		glBlendFunc( mat->getBlendSource(), mat->getBlendDestination() );
	}
}

//-----------------------------------//

void RenderDevice::undoRenderStateMaterial( Material* mat )
{
	if( mat->depthCompare != DepthCompare::Less )
		glDepthFunc( DepthCompare::Less );

	if( mat->isBlendingEnabled() ) 
		glDisable( GL_BLEND );

	if( !mat->cullBackfaces )
		glEnable( GL_CULL_FACE );

	if( mat->alphaTest )
		glDisable( GL_ALPHA_TEST );

	if( !mat->depthTest )
		glEnable( GL_DEPTH_TEST );

	if( !mat->depthWrite )
		glDepthMask( GL_TRUE );

	if( mat->lineSmooth )
		glDisable( GL_LINE_SMOOTH );

	if( mat->lineWidth != Material::DefaultLineWidth ) 
		glLineWidth( Material::DefaultLineWidth );

	unbindTextures(mat);
}

//-----------------------------------//

void RenderDevice::setView( RenderView* view )
{
	if( !view ) return;

	activeView = view;

	activeContext->setClearColor( view->getClearColor() );

	Vector2i origin = activeView->getOrigin();
	Vector2i size = activeView->getSize();

	glViewport( origin.x, origin.y, size.x, size.y );
}

//-----------------------------------//

void RenderDevice::clearView()
{
	#pragma TODO("Use a scissor test to clear only the view")

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glHasError("Could not clear the render target");
}

//-----------------------------------//

void RenderDevice::setRenderTarget(RenderTarget* target)
{
	if( !target || target == activeTarget )
		return;

	activeTarget = target;
	activeTarget->makeCurrent();

	activeContext = activeTarget->getContext().get();
}

//-----------------------------------//

RenderBuffer* RenderDevice::createRenderBuffer( const Settings& settings )
{
#if 0
	RenderBuffer* buffer( new FBO(settings) );
	renderTargets.push_back( buffer );
	
	return buffer;
#endif
	return nullptr;
}

//-----------------------------------//

bool RenderDevice::isFixedPipeline() const
{
	return pipeline == RenderPipeline::Fixed;
}

//-----------------------------------//

NAMESPACE_ENGINE_END

#endif