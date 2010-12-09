/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/Utilities.h"

#ifdef VAPOR_RENDERER_OPENGL

#include "vapor/render/Device.h"
#include "vapor/render/GL.h"
#include "vapor/render/FBO.h"
#include "vapor/render/Adapter.h"
#include "vapor/render/TextureManager.h"
#include "vapor/render/ProgramManager.h"
#include "vapor/render/Window.h"
#include "vapor/render/View.h"

#include "vapor/scene/Camera.h"
#include "vapor/scene/Entity.h"

namespace vapor {

//-----------------------------------//

RenderDevice::RenderDevice( ResourceManager* rm )
	: adapter(nullptr)
	, window(nullptr)
	, activeTarget(nullptr)
	, activeView(nullptr)
	, programManager(nullptr)
	, textureManager(nullptr)
	, shadowDepthBuffer(nullptr)
	, resourceManager(rm)
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
	textureManager = new TextureManager(resourceManager);
	programManager = new ProgramManager(resourceManager);

	setClearColor( Color::White );

	//glDepthMask( GL_TRUE );
	glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK );
	glEnable( GL_DEPTH_TEST );
	glDisable( GL_BLEND );
	glAlphaFunc( GL_EQUAL, 1 );
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

	if( !GLEW_VERSION_2_0 )
	{
		const char* str = "You need at least OpenGL 2.0 to run this.";
		System::messageDialog( str ); //, LogLevel::Error );
		exit( -1 );
	}
}

//-----------------------------------//

static bool RenderStateSorter(const RenderState& lhs, const RenderState& rhs)
{
	return lhs.group < rhs.group;
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
	const RenderablePtr& rend = state.renderable;

	if( !rend )
		return;

	const MaterialPtr& material = rend->getMaterial();

	if( !material )
		return;

	const ProgramPtr& program = material->getProgram();

	if( !program )
		return;

	rend->bind();
	setupRenderStateMaterial(material);

	if( !program->isLinked() )
		return;

	if( state.group != RenderStage::Overlays )
	{
		if( !setupRenderState(state) )
			return;

		if( !setupRenderStateLight(state, lights) )
			return;
	}
	else if( state.group == RenderStage::Overlays )
	{
		if( !setupRenderStateOverlay(state) )
			return;
	}

	state.callback();
	state.renderable->render( this );
	
	undoRenderStateMaterial(material);
	rend->unbind();
}

//-----------------------------------//

bool RenderDevice::setupRenderState( const RenderState& state )
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
	const ProgramPtr& program = state.renderable->getMaterial()->getProgram();
	
	glDisable( GL_DEPTH_TEST );
	//glDepthMask( false );

	Vector2i size = activeTarget->getSettings().getSize();

	Matrix4x4 proj = Matrix4x4::createOrthographicProjection( 
		0, size.x, 0, size.y, /*0,*/ 1.0, -1.0 );

	program->setUniform( "vp_ProjectionMatrix", proj );
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

void RenderDevice::setClearColor(const Color& newColor)
{
	if( newColor == color )
		return;

	color = newColor;
	glClearColor( color.r, color.g, color.b, color.a );
}

//-----------------------------------//

void RenderDevice::setView( RenderView* view )
{
	if( !view )
		return;

	activeView = view;

	Vector2i orig = activeView->getOrigin();
	Vector2i size = activeView->getSize();

	setClearColor( view->getClearColor() );
	glViewport( orig.x, orig.y, size.x, size.y );
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
	if( !target )
		return;

	if( activeTarget == target )
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