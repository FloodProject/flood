/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Graphics/API.h"

#ifdef ENABLE_RENDERER_OPENGL

#include "Graphics/Render.h"
#include "Core/Utilities.h"
#include "Graphics/RenderDevice.h"
#include "Graphics/RenderContext.h"
#include "Graphics/RenderBackend.h"
#include "Graphics/RenderView.h"
#include "Graphics/Program.h"

#include "Graphics/BufferManager.h"
#include "Graphics/ProgramManager.h"
#include "Graphics/TextureManager.h"

NAMESPACE_GRAPHICS_BEGIN

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
	, renderBackend(nullptr)
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
	#pragma TODO("Sort the render group by depth distance")

	// Sort the.renderables by render group.
	std::sort( queue.renderables.begin(), queue.renderables.end(), &RenderStateSorter );

	// Render all the.renderables in the queue.
	for( size_t i = 0; i < queue.renderables.size(); i++ )
	{
		const RenderState& state = queue.renderables[i];
		render(state, queue.lights);
	}
}

//-----------------------------------//

void RenderDevice::render( const RenderState& state, const LightQueue& lights )
{
#ifdef BUILD_DEBUG
	const RenderBatch* renderable = state.renderable;
	if( renderable ) return;

	const Material* material = state.material;
	if( !material ) return;
#endif

	switch(pipeline)
	{
	//case RenderPipeline::Fixed:
	//	setupRenderFixed(state, lights);
	//	return;
	case RenderPipeline::ShaderForward:
		setupRenderForward(state, lights);
		return;
	default:
		LogError("Unknown render pipeline");
		return;
	}
}

//-----------------------------------//

void RenderDevice::setupRenderForward(const RenderState& state, const LightQueue& lights)
{
	BufferManager* buffers = activeContext->bufferManager;
	ProgramManager* programs = activeContext->programManager;
	
	RenderBatch* renderable = state.renderable;
	bindBuffers(renderable);

	const GeometryBuffer* gb = renderable->getGeometryBuffer().get();
	if( gb->data.empty() ) return;

	BufferEntry* bufs = buffers->getBuffer(gb);

	// Setup the vertex buffer format.
	VertexBuffer* vb = bufs->vb.get();
	renderBackend->setupVertexBuffer(vb);
	
	Material* material = state.material;
	Shader* shader = material->getShader().Resolve();

	Program* program = programs->getProgram(shader);
	if( !program ) return;

	if( !program->isLinked() && !program->link() )
		return;

	program->bind();

	setupRenderStateMaterial(state, true);

	RenderLayer::Enum stage = renderable->getRenderLayer();

	if( renderable->onPreRender.empty() )
	{
		// Call the user pre render hook.
		renderable->onPreRender(activeView, state);
	}

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

	UniformBuffer* ub = renderable->getUniformBuffer().get();
	program->setUniforms(ub);

	render(renderable);
	
	if( renderable->onPostRender.empty() )
	{
		// Call the user post render hook.
		renderable->onPostRender(activeView, state);
	}
	
	undoRenderStateMaterial(material);
	
	program->unbind();

	renderBackend->unbindVertexBuffer(vb);
	unbindBuffers(renderable);
}
#if 0
//-----------------------------------//

void RenderDevice::render RenderBatch* renderable)
{	
	PrimitiveRasterMode::Enum mode =.renderable->getPrimitiveRasterMode();
	
	if( mode == PrimitiveRasterMode::Wireframe )
	{
		// Switch the OpenGL polygon mode to the one requested.
		glPrimitiveRasterMode( GL_FRONT_AND_BACK, PrimitiveRasterMode::Wireframe );
	}

	GLenum primitiveType = ConvertPrimitiveGL.renderable->getPrimitiveType());
	const GeometryBuffer* gb =.renderable->getGeometryBuffer().get();

	if( !gb->isIndexed() )
	{
		uint32 numVertices = gb->getSizeVertices();
		
		if( numVertices )
		{
			glDrawArrays( primitiveType, 0, numVertices );
 			CheckLastErrorGL("Error drawing vertex buffer");
		}
	}
	else
	{
		GLenum indexType = (gb->indexSize == 16) ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT;
		int numIndices = gb->indexData.size() / (gb->indexSize / 8);

		glDrawElements( primitiveType, numIndices, indexType, 0 );
		CheckLastErrorGL("Error drawing index buffer");
	}

	if( mode == PrimitiveRasterMode::Wireframe )
	{
		// Restore the polygon rendering mode.
		glPrimitiveRasterMode( GL_FRONT_AND_BACK, PrimitiveRasterMode::Solid );
	}
}

//-----------------------------------//

bool RenderDevice::setupRenderStateMatrix( const RenderState& state )
{
	const Matrix4x3& matModel = state.modelMatrix;
	const Matrix4x3& matView = activeView->viewMatrix;
	const Matrix4x4& matProjection = activeView->projectionMatrix;

	UniformBuffer* ub = state.renderable->getUniformBuffer().get();
	ub->setUniform( "vp_ModelMatrix", matModel );
	ub->setUniform( "vp_ViewMatrix", matView );
	ub->setUniform( "vp_ProjectionMatrix", matProjection );

	return true;
}

//-----------------------------------//

void RenderDevice::bindTextures(const RenderState& state, bool bindUniforms)
{
	TextureUnitMap& units = state.material->textureUnits;
	UniformBuffer* ub = state.renderable->getUniformBuffer().get();

	TextureUnitMap::const_iterator it;
	for( it = units.begin(); it != units.end(); it++ )
	{
		const TextureUnit& unit = it->second;
		
		const ImageHandle& handle = unit.image;
		Image* image = handle.Resolve();

		Texture* texture = activeContext->textureManager->getTexture(image).get();
		if( !texture ) continue;

		uint8 index = unit.unit;
		texture->bind(index);

		if(unit.overrideModes)
		{
			GLint filter = Texture::convertFilterFormat(unit.getFilterMode());
			glTexParameteri( texture->target, GL_TEXTURE_MIN_FILTER, filter );
			glTexParameteri( texture->target, GL_TEXTURE_MAG_FILTER, filter );

			GLint wrap = Texture::convertWrapFormat(unit.getWrapMode());
			glTexParameteri( texture->target, GL_TEXTURE_WRAP_S, wrap );
			glTexParameteri( texture->target, GL_TEXTURE_WRAP_T, wrap );
		}

		if( !bindUniforms ) continue;

		char s_TextureUniform[] = "vp_Texture0";
		size_t s_TextureUniformSize = ARRAY_SIZE(s_TextureUniform) - 1;

		// Build the uniform string without allocating memory.
		char indexChar = (index + '0');
		s_TextureUniform[s_TextureUniformSize] = indexChar;

		ub->setUniform( s_TextureUniform, (int32) index );
	}
}

//-----------------------------------//

void RenderDevice::unbindTextures(Material* material)
{
	TextureUnitMap& units = material->textureUnits;
	TextureManager* textureManager = activeContext->textureManager;

	TextureUnitMap::const_iterator it;
	for( it = units.begin(); it != units.end(); it++ )
	{
		const TextureUnit& unit = it->second;
		const ImageHandle& handle = unit.image;

		Texture* tex = textureManager->getTexture(handle.Resolve()).get();
		if( !tex ) continue;

		tex->unbind( it->first );
	}
}

//-----------------------------------//

bool RenderDevice::bindBuffers RenderBatch* renderable)
{
	BufferManager* buffers = activeContext->bufferManager;

	GeometryBuffer* gb =.renderable->getGeometryBuffer().get();
	VertexBuffer* vb = buffers->getVertexBuffer(gb).get();
	IndexBuffer* ib = buffers->getIndexBuffer(gb).get();
	
	if( !vb ) return false;

	if( !vb->isBuilt() || gb->needsRebuild )
	{
		// If the vertex buffer is not build yet, then we build it.
		vb->build(gb);
	}

	vb->bind();

	// If there is no index buffer associated with the geometry, we are done.
	if( !ib ) goto done;

	ib->bind();
	
	if( !ib->isBuilt || gb->needsRebuild )
	{
		// If the index buffer is not built, we also need to build it.
		ib->build(gb);
	}

done:

	gb->needsRebuild = false;
	return true;
}

//-----------------------------------//

bool RenderDevice::unbindBuffers RenderBatch* renderable)
{
	BufferManager* buffers = activeContext->bufferManager;
	const GeometryBuffer* gb =.renderable->getGeometryBuffer().get();
	
	VertexBuffer* vb = buffers->getVertexBuffer(gb).get();
	if( !vb ) return false;
	
	vb->unbind();

	IndexBuffer* ib = buffers->getIndexBuffer(gb).get();
	if( ib ) ib->unbind();

	return true;
}

//-----------------------------------//

void RenderDevice::updateLightDepth( LightState& state )
{
#if 0
	const LightPtr& light = state.light;
	assert( light->getLightType() == LightType::Directional );

	Texture* shadowDepthTexture;
	
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
#if 0
	if( lights.empty() ) return true;

	for( size_t i = 0; i < lights.size(); i++ )
	{
		LightState& state = lights[i];
		const Light* light = state.light;

		if( !light->getCastsShadows() )
			continue;

		updateLightDepth( state );
	}
#endif

	return true;
}

//-----------------------------------//

bool RenderDevice::setupRenderStateLight( const RenderState& state, const LightQueue& lights )
{
#if 0
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
#endif

	return true;
}

//-----------------------------------//

bool RenderDevice::setupRenderStateOverlay( const RenderState& state )
{
	Vector2i size = activeTarget->getSettings().getSize();
	Matrix4x4 projection = Matrix4x4::createOrthographic(0, size.x, size.y, 0, -100, 100);

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

	if( mat->depthRange != Vector2::UnitY )
		glDepthRange( mat->depthRange.x, mat->depthRange.y );

	if( mat->depthOffset != Vector2::Zero )
	{
		glPolygonOffset( mat->depthOffset.x, mat->depthOffset.y );
		glEnable(GL_POLYGON_OFFSET_FILL);
	}

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
	if( mat->isBlendingEnabled() ) 
		glDisable( GL_BLEND );

	if( !mat->cullBackfaces )
		glEnable( GL_CULL_FACE );

	if( mat->alphaTest )
		glDisable( GL_ALPHA_TEST );

	if( mat->depthCompare != DepthCompare::Less )
		glDepthFunc( DepthCompare::Less );

	if( mat->depthRange != Vector2::UnitY )
		glDepthRange(0, 1);

	if( mat->depthOffset != Vector2::Zero )
	{
		glPolygonOffset( 0, 0 );
		glDisable(GL_POLYGON_OFFSET_FILL);
	}

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

void RenderDevice::setActiveView( RenderView* view )
{
	activeView = view;

	if( !activeView ) return;

	activeContext->setClearColor( view->getClearColor() );

	const Vector2i& origin = activeView->getOrigin();
	const Vector2i& size = activeView->getSize();

	glViewport( origin.x, origin.y, size.x, size.y );
}

//-----------------------------------//

void RenderDevice::clearView()
{
	const Vector2i& origin = activeView->getOrigin();
	const Vector2i& size = activeView->getSize();

	//glScissor(origin.x, origin.y, size.x, size.y);

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	CheckLastErrorGL("Could not clear the render target");
}
#endif
//-----------------------------------//

void RenderDevice::setRenderTarget(RenderTarget* target)
{
	activeTarget = target;

	if( !activeTarget ) return;

	activeTarget->makeCurrent();
	activeContext = activeTarget->getContext();
	renderBackend = activeContext->backend;
}

//-----------------------------------//

bool RenderDevice::isFixedPipeline() const
{
	return pipeline == RenderPipeline::Fixed;
}

//-----------------------------------//

NAMESPACE_GRAPHICS_END

#endif