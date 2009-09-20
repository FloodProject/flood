/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include <vapor/PCH.h>

#include "Example.h"

#include <vapor/resources/Image.h>
#include <vapor/resources/MS3D.h>
#include <vapor/resources/Sound.h>
#include <vapor/resources/Shader.h>

#include <vapor/scene/Camera.h>
#include <vapor/scene/Sound.h>
#include <vapor/scene/Listener.h>
#include <vapor/scene/Geometry.h>

#include <vapor/input/InputManager.h>

#include <vapor/render/VertexBuffer.h>
#include <vapor/render/Texture.h>

#include <iostream>

using namespace vapor;
using namespace vapor::vfs;
using namespace vapor::log;
using namespace vapor::math;
using namespace vapor::scene;
using namespace vapor::audio;
using namespace vapor::render;
using namespace vapor::resources;
using namespace vapor::input;

//-----------------------------------//

Example::Example(const char** argv)
	: Framework("Example", argv), runLoop( false ), c( )
{

}

//-----------------------------------//

void Example::onInit()
{
	// Mount VFS points
	const std::string& media = "media";

	if ( !getVFS()->mount( media ) )
	{
		Log::MessageDialog( "Missing archive/directory '" + media + "'." );
	}

	//throw std::runtime_error( "" );

	inputMap = new InputMap(*getInputManager()); 
}

//-----------------------------------//

void Example::onSetupResources() 
{
	ResourceManager* rm = getResourceManager();
	
	ResourcePtr img = rm->createResource( "media/triton.png" );
	ResourcePtr sound = rm->createResource( "media/stereo.ogg" );

	ShaderPtr shader = tr1::static_pointer_cast< Shader > ( 
		rm->createResource( "media/shader.vs" ) );

	tex.reset( new Texture( tr1::static_pointer_cast< Image > ( img ) ) );
	(inputMap->registerAction("leap", vapor::input::Keys::Space))->bind(&Example::leap, this);
	(inputMap->registerAction("run", vapor::input::Keys::W))->bind(&Example::Run, this);
	inputMap->registerAction("leap", vapor::input::MouseButton::Left);
}

//-----------------------------------//

void Example::onSetupScene() 
{
	//ResourceManager* rm = getResourceManager();
	ScenePtr scene = getSceneManager();

	// Create a new Camera and position it to look at origin
	cam.reset( new Camera( getRenderDevice() ) );
	cam->translate( Vector3( 0.0f, 0.0f, -3.0f ) );
	cam->lookAt( Vector3.Zero );
	scene->add( cam );

	// Create a new VBO and upload triangle data
	VertexBufferPtr vb( new VertexBuffer() );

	std::vector< Vector3 > vertex;
	vertex.push_back( Vector3( 0.0f, 1.0f, 0.0f ) );
	vertex.push_back( Vector3( -1.0f, -1.0f, 0.0f ) );
	vertex.push_back( Vector3( 1.0f, -1.0f, 0.0f ) );

	std::vector< Vector3 > colors(3);
	std::fill( colors.begin(), colors.end(), Vector3( 1.0f, 0.0f, 0.0f ) );

	std::vector< Vector3 > coords;
	coords.push_back( Vector3( 0.0f, 1.0f, 0.0f ) );
	coords.push_back( Vector3( -1.0f, -1.0f, 0.0f ) );
	coords.push_back( Vector3( 1.0f, -1.0f, 0.0f ) );
	
	vb->set( VertexAttribute::Vertex, vertex );
	vb->set( VertexAttribute::Color, colors );
	//vb->set( VertexAttribute::MultiTexCoord0, coords );

	vb->build( BufferUsage::Static, BufferAccess::Write );

	// Create a new Renderable from the VBO and add it to a Geometry node
	RenderablePtr rend( new Renderable( Primitive::Triangles, vb ) );
	
	// Assign the renderable a material with a custom texture

	MaterialPtr mat( new Material( "SimpleMat" ) );
	mat->addTexture( tex );
	rend->setMaterial( mat );
	
	GeometryPtr geom( new Geometry( rend ) );
	
	//scene->add( geom );
}

//-----------------------------------//

void Example::onUpdate( double delta ) 
{
	ScenePtr scene = getSceneManager();
	scene->update();

	if( runLoop )
	{
		c.r += 0.0000001f / delta; c.r = (c.r > 1.0f) ? 0.0f : c.r;
		c.g += 0.0000003f / delta; c.b = (c.b > 1.0f) ? 0.0f : c.b;
		c.b += 0.0000007f / delta; c.g = (c.g > 1.0f) ? 0.0f : c.g;
	}
}

//-----------------------------------//

void Example::onRender()
{
	render::Device* device = getRenderDevice();

	device->setClearColor( c );
	device->clearTarget();
	
	cam->render();
}

//-----------------------------------//

void Example::onKeyPressed( const KeyEvent& keyEvent )
{
	//if( keyEvent.keyCode == Keys::Space )
	//	debug( "time: %f", timer.getDeltaTime() );

	if( keyEvent.keyCode == Keys::Pause )
		Log::showDebug = !Log::showDebug;

	if( keyEvent.keyCode == Keys::W )
		cam->translate( Vector3( 0.0f, 0.0f, 0.05f ) );

	if( keyEvent.keyCode == Keys::S )
		cam->translate( Vector3( 0.0f, 0.0f, -0.05f ) );

	if( keyEvent.keyCode == Keys::F )
		debug( "fps: %d", int( 1.0f / lastFrameTime ) );

	if( keyEvent.keyCode == Keys::G )
	{
		debug( "min/avg/max: %f / %f / %f", 
					minFrameTime, avgFrameTime, maxFrameTime );
	}

	//debug( "key press: %d", keyEvent.keyCode );
}

//-----------------------------------//

void Example::onButtonPressed( const MouseButtonEvent& btnEvent )
{
	if( btnEvent.button == MouseButton::Right )
		runLoop = !runLoop;

	//debug( "button press: %d", btnEvent.button );
}

void Example::leap()
{
	debug("Leap\n");
}

void Example::Run()
{
	debug("Run\n");
}
//-----------------------------------//
