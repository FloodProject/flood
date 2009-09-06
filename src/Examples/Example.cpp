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
	std::string media = "media";

	if ( !getVFS()->mount( media ) )
	{
		Log::MessageDialog( "Missing archive/directory '" + media + "'." );
	}

	// Register input devices callbacks
	Keyboard* kbd = getInputManager()->getKeyboard();
	Mouse* mouse = getInputManager()->getMouse();

	kbd->onKeyPress.bind( &Example::onKeyPressed, this );
	mouse->onMouseButtonPress.bind( &Example::onButtonPressed, this );
}

//-----------------------------------//

void Example::onSetupResources() 
{
	ResourceManager* rm = getResourceManager();
	
	ResourcePtr img = rm->createResource( "media/triton.png" );
	ResourcePtr sound = rm->createResource( "media/stereo.ogg" );

	ShaderPtr shader = tr1::static_pointer_cast< Shader > ( 
		rm->createResource( "media/shader.vs" ) );
	
	shader->compile();
}

//-----------------------------------//

void Example::onSetupScene() 
{
	//ResourceManager* rm = getResourceManager();
	ScenePtr scene = getSceneManager();

	// Create a new Camera and position it to look at origin
	cam.reset( new Camera( getRenderDevice() ) );
	cam->translate( Vector3( 0.0f, 0.0f, -1.0f ) );
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
	
	vb->set( VertexAttribute::Vertex, vertex );
	vb->set( VertexAttribute::Color, colors );

	vb->build( BufferUsage::Static, BufferAccess::Write );

	// Create a new Renderable from the VBO and add it to a Geometry node
	RenderablePtr rend( new Renderable( Primitive::Triangles, vb ) );
	GeometryPtr geom( new Geometry( rend ) );
	
	scene->add( geom );
}

//-----------------------------------//

void Example::onUpdate() 
{
	ScenePtr scene = getSceneManager();
	scene->update();

	if( runLoop )
	{
		c.r += 0.00001f; c.r = (c.r > 1.0f) ? 0.0f : c.r;
		c.g += 0.00003f; c.b = (c.b > 1.0f) ? 0.0f : c.b;
		c.b += 0.00007f; c.g = (c.g > 1.0f) ? 0.0f : c.g;
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

//-----------------------------------//
