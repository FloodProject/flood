/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"

#ifdef VAPOR_RENDERER_OPENGL

#include "render/Device.h"
#include "render/GL.h"
#include "render/FBO.h"
#include "render/Adapter.h"
#include "render/TextureManager.h"
#include "render/ProgramManager.h"
#include "render/Window.h"
#include "render/View.h"
#include "scene/Camera.h"
#include "scene/Entity.h"
#include "Utilities.h"
#include <algorithm>

namespace vapor {

//-----------------------------------//

BEGIN_ENUM(RenderPipeline)
	ENUM(Fixed)
	ENUM(ShaderForward)
	ENUM(ShaderDeferred)
END_ENUM()

//-----------------------------------//

RenderDevice::RenderDevice()
	: adapter(nullptr)
	, window(nullptr)
	, activeTarget(nullptr)
	, activeView(nullptr)
	, programManager(nullptr)
	, textureManager(nullptr)
	, shadowDepthBuffer(nullptr)
	, pipeline(RenderPipeline::Fixed)
{ }

//-----------------------------------//

RenderDevice::~RenderDevice()
{
	Log::info("Closing OpenGL rendering device");

	#pragma TODO("Confirm that all OpenGL resources were reclaimed on exit")	

	delete textureManager;
	delete programManager;
	delete adapter;
	delete window;
}

//-----------------------------------//

void RenderDevice::init()
{
	Log::info( "Creating OpenGL rendering device" );

	if( !window ) 
		Log::error( "No current OpenGL context found, stuff may fail" );
	
	checkExtensions();

	adapter = new Adapter();
	textureManager = new TextureManager();
	programManager = new ProgramManager();

	if( adapter->supportsShaders )
	{
		Log::info( "Shaders support detected. Switching to forward shaders pipeline" );
		//pipeline = RenderPipeline::ShaderForward;
	}

	setClearColor( Color::White );

	glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK );
	glEnable( GL_DEPTH_TEST );
	glDisable( GL_BLEND );
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
		Log::error( "Failed to initialize GLEW: %s", str );
		return;
	}

	Log::info( "Using GLEW version %s", glewGetString(GLEW_VERSION) );
}

//-----------------------------------//

static bool RenderStateSorter(const RenderState& lhs, const RenderState& rhs)
{
	return lhs.renderable->getRenderLayer() 
		< rhs.renderable->getRenderLayer();
}

void RenderDevice::render( RenderBlock& queue ) 
{
	//setupRenderStateShadow( queue.lights );

	// Sort the renderables by render group (TODO: use a radix sorter).
	std::sort( queue.renderables.begin(), queue.renderables.end(), &RenderStateSorter );

	// Render all the renderables in the queue.
	for( uint i = 0; i < queue.renderables.size(); i++ )
	{
		const RenderState& state = queue.renderables[i];
		render(state, queue.lights);
	}
}

//-----------------------------------//

void RenderDevice::render( const RenderState& state, const LightQueue& lights )
{
	const RenderablePtr& renderable = state.renderable;
	if( !renderable ) return;

	const MaterialPtr& material = renderable->getMaterial();
	if( !material ) return;

	switch(pipeline)
	{
	case RenderPipeline::Fixed:
		setupRenderFixed(state, lights);
		return;
	case RenderPipeline::ShaderForward:
		setupRenderForward(state, lights);
		return;
	default:
		Log::error("Unknown render pipeline");
		return;
	}
}

//-----------------------------------//

void RenderDevice::setupRenderFixed(const RenderState& state, const LightQueue& lights)
{
	const RenderablePtr& renderable = state.renderable;
	renderable->bind();

	const MaterialPtr& material = renderable->getMaterial();
	material->bindTextures(false);

	const VertexBufferPtr& vb = renderable->getVertexBuffer();
	vb->bindPointers();

	setupRenderStateMaterial(material);

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

	renderable->render(this);
	
	glPopMatrix();

	undoRenderStateMaterial(material);
	material->unbindTextures();

	vb->unbindPointers();	
	renderable->unbind();
}

//-----------------------------------//

bool RenderDevice::setupRenderFixedMatrix( const RenderState& state )
{
	CameraPtr camera = activeView->getCamera();

	if( !camera )
		return false;

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
	
	Matrix4x4 projection =
		Matrix4x4::createOrthographicProjection(0, size.x/*-1*/, size.y/*-1*/, 0, 0, 100);

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

	const MaterialPtr& material = renderable->getMaterial();
	const ProgramPtr& program = material->getProgram();

	if( !program )
		return;

	if( !program->isLinked() )
		program->link();
	
	program->bind();

	material->bindTextures(true);

	setupRenderStateMaterial(material);

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

	renderable->render(this);
	
	undoRenderStateMaterial(material);
	
	renderable->unbind();
	material->unbindTextures();
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

	const RenderablePtr& rend = state.renderable;
	const MaterialPtr& material = rend->getMaterial();
	const ProgramPtr& program = material->getProgram();

	program->setUniform( "vp_ModelMatrix", matModel );
	program->setUniform( "vp_ViewMatrix", matView );
	program->setUniform( "vp_ProjectionMatrix", matProjection );

	return true;
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

	for( uint i = 0; i < lights.size(); i++ )
	{
		LightState& state = lights[i];
		const LightPtr& light = state.light;

		if( !light->getCastsShadows() )
			continue;

		updateLightDepth( state );
	}

	return true;
}

//-----------------------------------//

bool RenderDevice::setupRenderStateLight( const RenderState& state, const LightQueue& lights )
{
	const RenderablePtr& rend = state.renderable;
	const MaterialPtr& material = rend->getMaterial();
	const ProgramPtr& program = material->getProgram();

	for( uint i = 0; i < lights.size(); i++ )
	{
		const LightState& lightState = lights[i];
		const LightPtr& light = lightState.light;

		//TexturePtr shadowDepthTexture;
		//shadowDepthTexture = shadowTextures[light];
		//assert( shadowDepthTexture != nullptr );

		std::vector< Color > colors;
		colors.push_back( light->getDiffuseColor() );
		colors.push_back( light->getSpecularColor() );
		colors.push_back( light->getEmissiveColor() );
		colors.push_back( light->getAmbientColor() );

		const TransformPtr& transform = lightState.transform;
		assert( transform != nullptr );

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
	Matrix4x4 projection = Matrix4x4::createOrthographicProjection(0, size.x, size.y, 0, 0, 100);

	const ProgramPtr& program = state.renderable->getMaterial()->getProgram();

	program->setUniform( "vp_ProjectionMatrix", projection );
	program->setUniform( "vp_ModelMatrix", state.modelMatrix );
	program->setUniform( "vp_ViewMatrix", Matrix4x4::Identity );
	program->setUniform( "vp_ModelViewMatrix", state.modelMatrix );

	return true;
}

//-----------------------------------//

void RenderDevice::setupRenderStateMaterial( const MaterialPtr& mat )
{
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

void RenderDevice::undoRenderStateMaterial( const MaterialPtr& mat )
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
}

//-----------------------------------//

Color RenderDevice::getPixel(ushort x, ushort y) const
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

Window* RenderDevice::createWindow( const WindowSettings& settings )
{
	Window* window = Window::createWindow( settings );
	assert( window != nullptr );

	setWindow( window );
	setRenderTarget( window );

	return window;
}

//-----------------------------------//

} // end namespace

#endif