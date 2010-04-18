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
#include "vapor/render/Adapter.h"
#include "vapor/render/TextureManager.h"
#include "vapor/render/ProgramManager.h"
#include "vapor/scene/Camera.h"

namespace vapor { namespace render {

using namespace vapor::math;
using namespace vapor::log;

//-----------------------------------//

Device::Device()
	: adapter(nullptr), window(nullptr), activeTarget(nullptr),
	programManager(nullptr), textureManager(nullptr)
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

	if( !activeTarget || !window ) 
	{
		error( "render::gl", "No current OpenGL context found, stuff may fail" );
	}

	checkExtensions();

	adapter = Adapter::getInstancePtr();
	textureManager = TextureManager::getInstancePtr();
	programManager = ProgramManager::getInstancePtr();

	//glEnable( GL_LINE_SMOOTH );
	//glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );

	glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK );

	setClearColor( Colors::White );

	//glEnable(GL_FOG);
	//glFogfv(GL_FOG_COLOR, g_fogColor);
	//glFogf(GL_FOG_DENSITY, g_fogDensity);
}

//-----------------------------------//

bool renderSorter(const RenderState& lhs, const RenderState& rhs)
{
	return lhs.group < rhs.group;
}

void Device::render( RenderBlock& queue, const scene::Camera* cam ) 
{
	glEnable( GL_DEPTH_TEST );

	// sort the list by render group
	// TODO: use a radix sorter
	std::sort( queue.renderables.begin(), queue.renderables.end(), &renderSorter );

	// render the list
	foreach( const RenderState& state, queue.renderables )
	{
		const RenderablePtr& rend = state.renderable;
		if( !rend ) continue;

		const MaterialPtr& material = rend->getMaterial();;
		if( !material ) continue;

		const ProgramPtr& program = material->getProgram();
		if( !program ) continue;

		rend->bind();

		// TODO: this needs some refactoring
		if( state.group != RenderGroup::Overlays )
		{
			//glDepthMask( true );

			program->setUniform( "vp_ProjectionMatrix", cam->getProjectionMatrix() );
			program->setUniform( "vp_ModelMatrix", state.modelMatrix );
			program->setUniform( "vp_ViewMatrix", cam->getViewMatrix() );
			program->setUniform( "vp_ModelViewMatrix", state.modelMatrix * cam->getViewMatrix() );

			if( queue.lights.size() > 0 )
			{
				std::vector<math::Color> lightColors;
				lightColors.push_back( queue.lights[0].light->diffuseColor );
				lightColors.push_back( queue.lights[0].light->specularColor );
				lightColors.push_back( queue.lights[0].light->emissiveColor );
				lightColors.push_back( queue.lights[0].light->ambientColor );

				{
					//PROFILE;
					program->setUniform( "vp_LightColors", lightColors );
					program->setUniform( "vp_LightDirection", Vector3(0.5f, 0.8f, 0.0f)/*queue.lights[0].transform->get*/ ); // TODO
				}
			}
		}
		else if( state.group == RenderGroup::Overlays )
		{
			glDisable( GL_DEPTH_TEST );
			//glDepthMask( false );

			const float w = static_cast<float>( activeTarget->getSettings().getWidth() );
			const float h = static_cast<float>( activeTarget->getSettings().getHeight() );

			Matrix4x4 proj = Matrix4x4::createOrthographicProjection( 
				-w/2, w/2, -h/2, h/2, -10.0, 10.0 );

			program->setUniform( "vp_ProjectionMatrix", proj );
			program->setUniform( "vp_ModelMatrix", state.modelMatrix );
			program->setUniform( "vp_ViewMatrix", math::Matrix4x4::Identity );
			program->setUniform( "vp_ModelViewMatrix", state.modelMatrix );
		}

		state.renderable->render( *this );

		rend->unbind();
	}
}

//-----------------------------------//

void Device::updateTarget()
{
	activeTarget->update();
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

void Device::clearTarget()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

//-----------------------------------//

void Device::setRenderTarget(RenderTargetPtr target)
{
	activeTarget = target;
	activeTarget->makeCurrent();
}

//-----------------------------------//

void Device::setWindowActiveTarget()
{
	setRenderTarget( window );
}

//-----------------------------------//

void Device::setViewport( const Vector2i& left,const Vector2i& size )
{
	if( (viewportLeft == left) && (viewportSize == size) )
		return;

	viewportLeft = left;
	viewportSize = size;

	glViewport( viewportLeft.x, viewportLeft.y, viewportSize.x, viewportSize.y );
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

WindowPtr Device::createWindow( const WindowSettings& settings )
{
	WindowPtr window = Window::createWindow( settings );

	setWindow( window );
	setRenderTarget( window );

	return window;
}

//-----------------------------------//

} } // end namespaces

#endif