/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"

#ifdef VAPOR_RENDERER_OPENGL

#include "vapor/render/Device.h"
#include "vapor/render/GL.h"
#include "vapor/render/FBO.h"
#include "vapor/render/Adapter.h"
#include "vapor/render/TextureManager.h"
#include "vapor/render/ProgramManager.h"

#include "vapor/scene/Camera.h"
#include "vapor/scene/Node.h"

namespace vapor { namespace render {

using namespace vapor::log;
using namespace vapor::math;
using namespace vapor::scene;
using namespace vapor::resources;

//-----------------------------------//

Device::Device():
	adapter(nullptr),
	window(nullptr),
	activeTarget(nullptr),
	programManager(nullptr),
	textureManager(nullptr),
	shadowDepthBuffer(nullptr)
{
}

//-----------------------------------//

Device::~Device()
{
	info("render::opengl", "Closing OpenGL rendering device");

	// TODO: delete all OpenGL resources (shaders, textures...)
	// Or make sure they are all deleted once we delete the OpenGL context.

	delete textureManager;
	delete programManager;
	delete adapter;
	delete window;
}

//-----------------------------------//

void Device::init()
{
	info( "render::gl", "Creating OpenGL rendering device" );

	if( !window ) 
	{
		error( "render::gl", "No current OpenGL context found, stuff may fail" );
	}

	checkExtensions();

	adapter = new Adapter();
	textureManager = new TextureManager();
	programManager = new ProgramManager();

	setClearColor( Color::White );

	//glEnable( GL_DEPTH_TEST );
	//glDepthMask( GL_TRUE );

	glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK );
	glDisable( GL_BLEND );
	//glEnable( GL_LINE_SMOOTH );
	//glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
}

//-----------------------------------//

void Device::checkExtensions()
{
	// Initialize GLEW (OpenGL Extension Wrangler) and check that
	// the user supports the minimum required OpenGL version.
	GLenum err = glewInit();
	
	if( err != GLEW_OK )
	{
		const GLubyte* str = glewGetErrorString(err);
		error( "render", "Failed to initialize GLEW: %s", str );
		return;
	}

	info( "render", "Using GLEW version %s", glewGetString(GLEW_VERSION) );

	if( !GLEW_VERSION_2_0 )
	{
		const char* str = "You need at least OpenGL 2.0 to run this.";
		Log::MessageDialog( str, log::LogLevel::Error );
		exit( -1 ); // TODO: exit program in a structured manner
	}
}

//-----------------------------------//

bool stateSorter(const RenderState& lhs, const RenderState& rhs)
{
	return lhs.group < rhs.group;
}

void Device::render( RenderBlock& queue, const Camera* cam ) 
{
	assert( cam != nullptr );
	camera = cam;

	//setupRenderStateShadow( queue.lights );

	// Sort the renderables by render group (TODO: use a radix sorter).
	std::sort( queue.renderables.begin(), queue.renderables.end(), &stateSorter );

	// Render all the renderables in the queue.
	foreach( const RenderState& state, queue.renderables )
	{
		const RenderablePtr& rend = state.renderable;
		
		if( !rend ) 
			continue;

		const MaterialPtr& material = rend->getMaterial();

		if( !material )
			continue;

		const ProgramPtr& program = material->getProgram();

		if( !program )
			continue;

		rend->bind();
		setupRenderStateMaterial(material);

		if( !program->isLinked() )
			continue;

		if( state.group != RenderGroup::Overlays )
		{
			if( !setupRenderState(state, cam) )
				continue;

			if( !setupRenderStateLight(state, queue.lights) )
				continue;
		}
		else if( state.group == RenderGroup::Overlays )
		{
			if( !setupRenderStateOverlay(state) )
				continue;
		}

		state.renderable->render( *this );
		
		undoRenderStateMaterial(material);
		rend->unbind();
	}
}

//-----------------------------------//

bool Device::setupRenderState( const RenderState& state, const Camera* cam )
{
	const RenderablePtr& rend = state.renderable;
	const MaterialPtr& material = rend->getMaterial();
	const ProgramPtr& program = material->getProgram();

	program->setUniform( "vp_ProjectionMatrix", cam->getProjectionMatrix() );
	program->setUniform( "vp_ModelMatrix", state.modelMatrix );
	program->setUniform( "vp_ViewMatrix", cam->getViewMatrix() );
	program->setUniform( "vp_ModelViewMatrix", state.modelMatrix * cam->getViewMatrix() );

	glEnable( GL_DEPTH_TEST );

	return true;
}

//-----------------------------------//

void Device::updateLightDepth( LightState& state )
{
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
	
	NodePtr lightCameraNode( new Node("ShadowCamera") );
	lightCameraNode->addTransform(); /*Component( lightTransform );*/
	lightCameraNode->addComponent( lightCamera );

	ViewportPtr lightView = new Viewport(lightCamera, shadowDepthBuffer);

	if( !shadowDepthBuffer->check() )
		return;

	// TODO: turn off color writes (glColorMask?)
	lightView->update();
	shadowDepthBuffer->unbind();

	Matrix4x4 bias; bias.identity();
	bias.m11 = 0.5f;
	bias.m22 = 0.5f;
	bias.m33 = 0.5f;
	bias.tx  = 0.5f;
	bias.ty  = 0.5f;
	bias.tz  = 0.5f;

	state.projection = lightCamera->getViewMatrix()
		* lightCamera->getProjectionMatrix() * bias;
}

//-----------------------------------//

bool Device::setupRenderStateShadow( LightQueue& lights )
{
	if( lights.empty() ) 
		return true;

	foreach( LightState& state, lights )
	{
		const LightPtr& light = state.light;

		if( !light->getCastsShadows() )
			continue;

		updateLightDepth( state );
	}

	return true;
}

//-----------------------------------//

bool Device::setupRenderStateLight( const RenderState& state, const LightQueue& lights )
{
	const RenderablePtr& rend = state.renderable;
	const MaterialPtr& material = rend->getMaterial();
	const ProgramPtr& program = material->getProgram();

	if( lights.empty() ) return true;

	foreach( const LightState& lightState, lights )
	{
		const LightPtr& light = lightState.light;

		//TexturePtr shadowDepthTexture;
		//shadowDepthTexture = shadowTextures[light];
		//assert( shadowDepthTexture != nullptr );

		std::vector< Color > colors;
		colors.push_back( light->getDiffuseColor() );
		colors.push_back( light->getSpecularColor() );
		colors.push_back( light->getEmissiveColor() );
		colors.push_back( light->getAmbientColor() );

		//const TransformPtr& transform = lightState.transform;
		//assert( transform != nullptr );

		// TODO: fix the lighting stuff
		program->setUniform( "vp_LightColors", colors );
		program->setUniform( "vp_LightDirection", -Vector3::UnitY/*transform->getRotation()*/ );
		//program->setUniform( "vp_ShadowMap", shadowDepthTexture->id() );
		//program->setUniform( "vp_CameraProj", state.modelMatrix * lightState.projection );
	}

	return true;
}

//-----------------------------------//

bool Device::setupRenderStateOverlay( const RenderState& state )
{
	const ProgramPtr& program =
		state.renderable->getMaterial()->getProgram();
	
	glDisable( GL_DEPTH_TEST );
	//glDepthMask( false );

	Vector2i size = activeTarget->getSettings().getSize();

	Matrix4x4 proj = Matrix4x4::createOrthographicProjection( 
		0, size.x, 0, size.y, -10.0, 10.0 );

	program->setUniform( "vp_ProjectionMatrix", proj );
	program->setUniform( "vp_ModelMatrix", state.modelMatrix );
	program->setUniform( "vp_ViewMatrix", Matrix4x4::Identity );
	program->setUniform( "vp_ModelViewMatrix", state.modelMatrix );

	return true;
}

//-----------------------------------//

void Device::setupRenderStateMaterial( const MaterialPtr& mat )
{
	if( mat->lineSmooth )
		glEnable( GL_LINE_SMOOTH );
	
	if( mat->lineWidth != Material::DefaultLineWidth )
		glLineWidth( mat->getLineWidth() );

	if( !mat->cullBackfaces )
		glDisable( GL_CULL_FACE );

	if( !mat->depthTest )
		glDisable( GL_DEPTH_TEST );

	if( !mat->depthWrite )
		glDepthMask( GL_FALSE );

	if( mat->isBlendingEnabled() ) 
	{
		glEnable( GL_BLEND );
		glBlendFunc( mat->getBlendingSource(), mat->getBlendingDestination() );
	}
}

//-----------------------------------//

void Device::undoRenderStateMaterial( const MaterialPtr& mat )
{
	if( mat->isBlendingEnabled() ) 
		glDisable( GL_BLEND );

	if( !mat->cullBackfaces )
		glEnable( GL_CULL_FACE );

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

Color Device::getPixel(ushort x, ushort y) const
{
	Color pick;
	
	glReadPixels(x, y, 1, 1, GL_RGBA, GL_FLOAT, &pick);

	if( glHasError("Error reading pixels from framebuffer") )
		return Color::White;
	
	return pick;
}

//-----------------------------------//

void Device::setClearColor(const math::Color& newColor)
{
	if( newColor == color )
		return;

	color = newColor;
	glClearColor( color.r, color.g, color.b, color.a );
}

//-----------------------------------//

void Device::setViewport( const Vector2i& newLeft, const Vector2i& newSize )
{
	if( (viewportLeft == newLeft) && (viewportSize == newSize) )
		return;

	viewportLeft = newLeft;
	viewportSize = newSize;

	glViewport( viewportLeft.x, viewportLeft.y, viewportSize.x, viewportSize.y );
}

//-----------------------------------//

void Device::clearTarget()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glHasError("Could not clear the render target");
}

//-----------------------------------//

void Device::setRenderTarget(RenderTargetPtr target)
{
	activeTarget = target;

	if(activeTarget)
		activeTarget->makeCurrent();
}

//-----------------------------------//

void Device::setWindowActiveTarget()
{
	setRenderTarget( window );
}

//-----------------------------------//

RenderBufferPtr Device::createRenderBuffer( const Settings& settings )
{
	RenderBufferPtr buffer( new FBO(settings) );
	renderTargets.push_back( buffer );
	
	return buffer;
}

//-----------------------------------//

WindowPtr Device::createWindow( const WindowSettings& settings )
{
	WindowPtr window = Window::createWindow( settings );
	assert( window != nullptr );

	setWindow( window );
	setRenderTarget( window );

	return window;
}

//-----------------------------------//

} } // end namespaces

#endif