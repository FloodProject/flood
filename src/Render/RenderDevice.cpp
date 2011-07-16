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
#include "Render/GL.h"
#include "Render/FBO.h"
#include "Render/Adapter.h"
#include "Render/TextureManager.h"
#include "Render/ProgramManager.h"
#include "Render/Window.h"
#include "Render/View.h"

#include "Scene/Entity.h"
#include "Scene/Transform.h"
#include "Scene/Camera.h"
#include "Scene/Light.h"

namespace vapor {

//-----------------------------------//

REFLECT_ENUM(RenderPipeline)
	ENUM(Fixed)
	ENUM(ShaderForward)
	ENUM(ShaderDeferred)
REFLECT_ENUM_END()

//-----------------------------------//

static RenderDevice* gs_RenderDevice = nullptr;
RenderDevice* GetRenderDevice() { return gs_RenderDevice; }

RenderDevice::RenderDevice()
	: adapter(nullptr)
	, window(nullptr)
	, activeTarget(nullptr)
	, activeView(nullptr)
	, programManager(nullptr)
	, textureManager(nullptr)
	, shadowDepthBuffer(nullptr)
	, pipeline(RenderPipeline::Fixed)
	, initDone(false)
{
	gs_RenderDevice = this;
}

//-----------------------------------//

RenderDevice::~RenderDevice()
{
	LogInfo("Closing OpenGL rendering device");

	#pragma TODO("Confirm that all OpenGL resources were reclaimed on exit")	

	Deallocate(textureManager);
	Deallocate(programManager);
	Deallocate(adapter);
	window = nullptr;
}

//-----------------------------------//

void RenderDevice::init()
{
	if( initDone ) return;

	LogInfo( "Creating OpenGL rendering device" );

	if( !window ) LogError( "No current OpenGL context found, stuff may fail" );
	
	checkExtensions();

	adapter = Allocate(Adapter, AllocatorGetHeap());
	checkCapabilities(adapter);
	showCapabilities(adapter);

	textureManager = Allocate(TextureManager, AllocatorGetHeap());
	programManager = Allocate(ProgramManager, AllocatorGetHeap());

	if( adapter->supportsShaders )
	{
		LogInfo( "Shaders support detected. Switching to forward shaders pipeline" );
		pipeline = RenderPipeline::ShaderForward;
	}

	resetState();

	initDone = true;
}

//-----------------------------------//

void RenderDevice::resetState()
{
	setClearColor( Color::White );

	glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK );

	glEnable( GL_DEPTH_TEST );
	glDisable( GL_BLEND );

	if(adapter->supportsFixedPipeline)
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
	}

	if(adapter->supportsVertexBuffers)
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	if(adapter->supportsShaders)
	{
		glUseProgram(0);
	}

	for(int32 i = 0; i < adapter->maxTextureUnits; i++)
	{
		glActiveTexture( GL_TEXTURE0+i );
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

//-----------------------------------//

void RenderDevice::checkCapabilities(Adapter* adapter)
{
	adapter->supportsShaders = !! GLEW_ARB_shading_language_100;
	adapter->supportsVertexBuffers = !! GLEW_ARB_vertex_buffer_object;
	adapter->supportsAnisotropic = !! GLEW_EXT_texture_filter_anisotropic;
	
	glGetIntegerv( GL_MAX_TEXTURE_SIZE, (GLint*) &adapter->maxTextureSize );
	glGetIntegerv( GL_MAX_TEXTURE_IMAGE_UNITS, (GLint*) &adapter->maxTextureUnits );
	glGetIntegerv( GL_MAX_VERTEX_ATTRIBS, (GLint*) &adapter->maxAttribs );

	// get the name of the card
	adapter->name = (const char*) glGetString(GL_RENDERER);
	adapter->vendor = (const char*) glGetString(GL_VENDOR);
	adapter->version = (const char*) glGetString(GL_VERSION);

	if(adapter->supportsShaders)
		adapter->shading = (const char*) glGetString(GL_SHADING_LANGUAGE_VERSION);

	adapter->supportsFixedPipeline = true;
}

//-----------------------------------//

void RenderDevice::showCapabilities(Adapter* card) 
{
	if( card->name.empty() ) return;

	LogInfo( "Graphics adapter: %s", card->name.c_str() );

	LogInfo( "%s%s%s", 
		!card->version.empty() ? ("OpenGL " + card->version).c_str() : "",
		!card->shading.empty() ? (" / GLSL " + card->shading).c_str() : "",
		!card->driver.empty() ? (" / driver: " + card->driver).c_str() : "" );

	LogInfo("Support vertex buffers: ", card->supportsVertexBuffers ? "yes" : "no" );

	LogInfo( "Max texture size: %dx%d", card->maxTextureSize, card->maxTextureSize );
	LogInfo( "Max texture units: %d", card->maxTextureUnits );
	LogInfo( "Max vertex attributes: %d", card->maxAttribs );
}

//-----------------------------------//

void RenderDevice::checkExtensions()
{
	// Initialize GLEW (OpenGL Extension Wrangler) and check that
	// the user supports the minimum required OpenGL version.
	GLenum err = glewInit();
	
	if( err != GLEW_OK )
	{
		const GLubyte* str = glewGetErrorString(err);
		LogError( "Failed to initialize GLEW: %s", str );
		return;
	}

	LogInfo( "Using GLEW version %s", glewGetString(GLEW_VERSION) );
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
	setupRenderStateMaterial(material, false);

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
	
	Matrix4x4 projection = Matrix4x4::createOrthographic(0, size.x/*-1*/, size.y/*-1*/, 0, 0, 100);

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
	const ProgramPtr& program = programManager->getProgram(name);
	if( !program ) return;

	if( !program->isLinked() )
	{
		program->link();
	}

	program->bind();

	setupRenderStateMaterial(material, true);

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
	assert( activeView != nullptr );
	CameraPtr camera = activeView->getCamera();

	if( !camera )
		return false;

	const Frustum& frustum = camera->getFrustum();

	const Matrix4x3& matModel = state.modelMatrix;
	const Matrix4x3& matView = camera->getViewMatrix();
	const Matrix4x4& matProjection = frustum.matProjection;

	Material* material = state.material;
	const String& name = material->getProgram();
	const ProgramPtr& program = programManager->getProgram(name);

	program->setUniform( "vp_ModelMatrix", matModel );
	program->setUniform( "vp_ViewMatrix", matView );
	program->setUniform( "vp_ProjectionMatrix", matProjection );

	return true;
}

//-----------------------------------//

void RenderDevice::bindTextures(Material* material, bool bindUniforms)
{
	TextureUnitMap& units = material->textureUnits;

	const ProgramPtr& program = programManager->getProgram(material->getProgram());

	TextureUnitMap::const_iterator it;
	for( it = units.begin(); it != units.end(); it++ )
	{
		int32 index = it->first;
		const ImageHandle& image = it->second;

		const TexturePtr& texture = textureManager->getTexture(image.Resolve());
		texture->bind(index);

		if( !bindUniforms ) continue;
		if( !program ) continue;

		String uniform = "vp_Texture" + StringFromNumber(index);
		program->setUniform( uniform, index );
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
		const TexturePtr& tex = textureManager->getTexture(handle.Resolve());
		tex->unbind( it->first );
	}
}

//-----------------------------------//

void RenderDevice::updateLightDepth( LightState& state )
{
	//const LightPtr& light = state.light;
	//assert( light->getLightType() == LightType::Directional );

	//TexturePtr shadowDepthTexture;
	//
	//if( !shadowDepthBuffer )
	//{
	//	shadowDepthBuffer = createRenderBuffer( Settings(512, 512) );
	//}

	//if( shadowTextures.find(light) == shadowTextures.end() )
	//{
	//	shadowDepthTexture = shadowDepthBuffer->createRenderTexture();
	//	shadowTextures[light] = shadowDepthTexture;
	//}
	//else
	//{
	//	shadowDepthTexture = shadowTextures[light];
	//}

	//CameraPtr lightCamera( new Camera(*camera) );
	//TransformPtr lightTransform( new Transform(*state.transform.get()) );
	//
	//EntityPtr lightCameraEntity( new Entity("ShadowCamera") );
	//lightCameraEntity->addTransform(); /*Component( lightTransform );*/
	//lightCameraEntity->addComponent( lightCamera );

	//RenderView* lightView = new View(lightCamera, shadowDepthBuffer);

	//if( !shadowDepthBuffer->check() )
	//	return;

	//// TODO: turn off color writes (glColorMask?)
	//lightView->update();
	//shadowDepthBuffer->unbind();

	//Matrix4x4 bias;
	//bias.identity();
	//bias.m11 = 0.5f;
	//bias.m22 = 0.5f;
	//bias.m33 = 0.5f;
	//bias.tx  = 0.5f;
	//bias.ty  = 0.5f;
	//bias.tz  = 0.5f;

	//const Frustum& frustum = lightCamera->getFrustum();
	//const Matrix4x4& matProjection = frustum.projectionMatrix;

	//state.projection = lightCamera->getViewMatrix()
	//	* matProjection * bias;
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
	Material* material = state.material;
	const ProgramPtr& program = programManager->getProgram(material->getProgram());

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
		program->setUniform( "vp_LightColors", colors );
		program->setUniform( "vp_LightDirection", transform->getRotationMatrix() );
		//program->setUniform( "vp_ShadowMap", shadowDepthTexture->id() );
		//program->setUniform( "vp_CameraProj", state.modelMatrix * lightState.projection );
	}

	return true;
}

//-----------------------------------//

bool RenderDevice::setupRenderStateOverlay( const RenderState& state )
{
	Vector2i size = activeTarget->getSettings().getSize();
	Matrix4x4 projection = Matrix4x4::createOrthographic(0, size.x, size.y, 0, 0, 100);

	const String& name = state.renderable->getMaterial().Resolve()->getProgram();
	const ProgramPtr& program = programManager->getProgram(name);

	program->setUniform( "vp_ProjectionMatrix", projection );
	program->setUniform( "vp_ModelMatrix", state.modelMatrix );
	program->setUniform( "vp_ViewMatrix", Matrix4x4::Identity );
	program->setUniform( "vp_ModelViewMatrix", state.modelMatrix );

	return true;
}

//-----------------------------------//

void RenderDevice::setupRenderStateMaterial( Material* mat, bool bindUniforms )
{
	bindTextures(mat, bindUniforms);

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

Color RenderDevice::getPixel(uint16 x, uint16 y) const
{
	Color pick;
	
	glReadPixels(x, y, 1, 1, GL_RGBA, GL_FLOAT, &pick);

	if( glHasError("Error reading pixels from framebuffer") )
		return Color::White;
	
	return pick;
}

//-----------------------------------//

void RenderDevice::setClearColor(const Color& color)
{
	glClearColor( color.r, color.g, color.b, color.a );
}

//-----------------------------------//

void RenderDevice::setView( RenderView* view )
{
	if( !view ) return;

	activeView = view;

	setClearColor( view->getClearColor() );

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
}

//-----------------------------------//

RenderBuffer* RenderDevice::createRenderBuffer( const Settings& settings )
{
	RenderBuffer* buffer( new FBO(settings) );
	renderTargets.push_back( buffer );
	
	return buffer;
}

//-----------------------------------//

bool RenderDevice::isFixedPipeline() const
{
	return pipeline == RenderPipeline::Fixed;
}

//-----------------------------------//

} // end namespace

#endif