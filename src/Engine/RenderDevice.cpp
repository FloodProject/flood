/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#ifdef VAPOR_RENDERER_OPENGL

#include "vapor/render/Device.h"
#include "vapor/render/Adapter.h"
#include "vapor/render/BufferManager.h"
#include "vapor/scene/Camera.h"

#include "vapor/render/GL.h"

using namespace vapor::math;

namespace vapor {
	namespace render {

//-----------------------------------//

Device::Device()
	: clearColor( math::Colors::White ), adapter( nullptr ),
	window( nullptr ), bufferManager( nullptr ), activeTarget( nullptr )
{

}

//-----------------------------------//

Device::~Device()
{
	info("render::opengl", "Closing OpenGL rendering device");

	// TODO: delete all OpenGL resources (shaders, textures...)

	delete adapter;
	delete window;
	delete bufferManager;
}

//-----------------------------------//

void Device::init()
{
	info( "render::gl", "Creating OpenGL rendering device" );

	if( !activeTarget || !window ) 
	{
		warn( "render::gl", "No current OpenGL context found, stuff may fail" );
	}

	checkExtensions();

	adapter = new Adapter();
	bufferManager = new BufferManager();
	TextureManager::getInstance();
	ProgramManager::getInstance();

	//glEnable( GL_LINE_SMOOTH );

	//glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK );
}

//-----------------------------------//

Window* Device::getWindow() const
{
	return window;
}

//-----------------------------------//

void Device::setWindow( Window *window )
{
	this->window = window;;
}

//-----------------------------------//

Window& Device::createWindow( const WindowSettings& settings )
{
	//const_cast<WindowSettings&>(settings).setAntiAliasing( 4 );

	Window& window = Window::createWindow( settings );

	setWindow( &window );
	setRenderTarget( &window );

	return window;
}

//-----------------------------------//

Adapter* Device::getAdapter() const
{
	return adapter;
}

//-----------------------------------//

void Device::setClearColor(math::Color c)
{
	clearColor = c;
}

//-----------------------------------//

void Device::updateTarget()
{
	activeTarget->update();
}

//-----------------------------------//

void Device::setRenderTarget(RenderTarget* renderTarget)
{
	activeTarget = renderTarget;
}

//-----------------------------------//

BufferManager* Device::getBufferManager() const
{
	return bufferManager;
}

//-----------------------------------//

TextureManager* Device::getTextureManager() const
{
	return TextureManager::getInstancePtr();
}

//-----------------------------------//

bool renderSorter(const RenderState& lhs, const RenderState& rhs)
{
	return lhs.group < rhs.group;
}

void Device::render( RenderQueue& queue, const scene::Camera* cam ) 
{
	activeTarget->makeCurrent();

	glEnable( GL_DEPTH_TEST );
	glClear( GL_DEPTH_BUFFER_BIT );

	// sort the list by render group
	// TODO: use a radix sorter
	std::sort( queue.begin(), queue.end(), &renderSorter );

	// render the list
	foreach( RenderState state, queue )
	{
		ProgramPtr program = state.renderable->getMaterial()->getProgram();

		if( state.group == RenderGroup::Normal )
		{
			//glDepthMask( true );

			program->setUniform( "vp_ProjectionMatrix", cam->getProjectionMatrix() );
			program->setUniform( "vp_ModelMatrix", state.modelMatrix );
			program->setUniform( "vp_ViewMatrix", cam->getViewMatrix() );
			program->setUniform( "vp_ModelViewMatrix", state.modelMatrix * cam->getViewMatrix() );
		}
		else if( state.group == RenderGroup::Overlays )
		{
			glDisable( GL_DEPTH_TEST );
			//glDepthMask( false );

			const float w = activeTarget->getSettings().getWidth();
			const float h = activeTarget->getSettings().getHeight();

			Matrix4x4 proj = Matrix4x4::createOrthographicProjection( 
				-w/2, w/2, -h/2, h/2, -10.0, 10.0 );

			program->setUniform( "vp_ProjectionMatrix", proj );
			program->setUniform( "vp_ModelMatrix", state.modelMatrix );
			program->setUniform( "vp_ViewMatrix", math::Matrix4x4::Identity );
			program->setUniform( "vp_ModelViewMatrix", state.modelMatrix );
		}

		state.renderable->render( *this );
	}
}

//-----------------------------------//

void Device::checkExtensions()
{
	// init GLEW (OpenGL Extension Wrangler)
	GLenum err = glewInit();
	
	if( err != GLEW_OK )
	{
		error( "render::opengl", 
			"Failed to initialize GLEW: %s", glewGetErrorString( err ) );

		return;
	}

	info( "render::opengl", "Using GLEW version %s", 
		glewGetString( GLEW_VERSION ) );

	//if( !GLEW_VERSION_2_1 )
	//{
	//	log::Log::MessageDialog( "You need at least OpenGL 2.1 to run this.",
	//		log::LogLevel::Error );

	//	exit( 1 );
	//	
	//	// TODO: exit program in a structured manner
	//}
}

//-----------------------------------//

RenderTarget* Device::getRenderTarget() const
{
	return activeTarget;
}

//-----------------------------------//

//RenderDevice::render( RenderQueue& queue, Matrix4x4 invCamera = Matrix4x4::Identity ) // ?
//{
//    // 1. sort renderables by state
//    sortByMaterial( queue );
//        
//    // 2. for each node render
//    foreach ( renderable in queue )
//    {
//        // change state if needed
//        changeState( renderable );
//        
//        // concantenate camera and renderable matrices
//        glLoadMatrix( renderable->second * invCamera );
//        
//        // render it
//        renderable->first.render( *this );
//    }
//}

//-----------------------------------//

void Device::clearTarget()
{
	glClearColor( clearColor.r, clearColor.g, clearColor.b, clearColor.a );
	glClear( GL_COLOR_BUFFER_BIT );
}

//-----------------------------------//

} } // end namespaces

#endif