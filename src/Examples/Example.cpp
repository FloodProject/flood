/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include <vapor/PCH.h>

#include "Example.h"

//-----------------------------------//

Example::Example(const char** argv)
	: Framework("Example", argv), runLoop( false ), c( 0.3f, 0.0f, 0.8f )
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
}

//-----------------------------------//

void Example::onSetupResources() 
{
	ResourceManager* rm = getResourceManager();
	
	ImagePtr img = rm->loadResource< Image >( "media/triton.png" );
	snd = rm->loadResource< resources::Sound >( "media/stereo.ogg" );

	tex.reset( new Texture( img ) );
}

//-----------------------------------//

void Example::onSetupScene() 
{
	ScenePtr scene = getSceneManager();

	GroupPtr grp( new Group() );

	ListenerPtr ls( new Listener( getAudioDevice() ) );
	ls->makeCurrent();
	grp->add( ls );

	sound.reset( new scene::Sound( ls, snd) );
	grp->add( sound );
	scene->add( grp );

	// Create a new Camera and position it to look at origin
	cam.reset( new Camera( getRenderDevice() ) );
	cam->translate( Vector3( 0.0f, 0.0f, -3.0f ) );
	cam->lookAt( Vector3::Zero );
	scene->add( cam );

	buildTriangle();
}

//-----------------------------------//

void Example::buildTriangle()
{
	ResourceManager* rm = getResourceManager();

	// Create a new VBO and upload triangle data
	VertexBufferPtr vb( new VertexBuffer() );

	// Vertex position data
	std::vector< Vector3 > vertex;
	vertex.push_back( Vector3( 0.0f, 1.0f, 0.0f ) );
	vertex.push_back( Vector3( -1.0f, -1.0f, 0.0f ) );
	vertex.push_back( Vector3( 1.0f, -1.0f, 0.0f ) );

	// Vertex color data
	std::vector< Vector3 > colors;
	colors.push_back( Vector3( 0.0f, 1.0f, 0.0f ) );
	colors.push_back( Vector3( 0.0f, 0.0f, 1.0f ) );
	colors.push_back( Vector3( 1.0f, 0.0f, 0.0f ) );

	// Vertex tex coords data
	std::vector< Vector3 > coords;
	coords.push_back( Vector3( 0.0f, 1.0f, 0.0f ) );
	coords.push_back( Vector3( -1.0f, -1.0f, 0.0f ) );
	coords.push_back( Vector3( 1.0f, -1.0f, 0.0f ) );

	// Vertex buffer setup
	vb->set( VertexAttribute::Vertex, vertex );
	vb->set( VertexAttribute::Color, colors );
	vb->set( VertexAttribute::MultiTexCoord0, coords );

	vb->build( BufferUsage::Static, BufferAccess::Write );

	ProgramPtr program( new GLSL_Program( 
			rm->loadResource< GLSL_Shader >( "media/shader.vs" ),
			rm->loadResource< GLSL_Shader >( "media/shader.fs" ) ) );

	// Assign the renderable a material with a custom texture
	MaterialPtr mat( new Material( "SimpleMat", program ) );
	mat->addTexture( 0, tex );
	
	// Create a new Renderable from the VBO and add it to a Geometry node
	RenderablePtr rend( new Renderable( Primitive::Triangles, vb, mat ) );

	GeometryPtr geom( new Geometry( rend ) );
	getSceneManager()->add( geom );
}

//-----------------------------------//

void Example::onUpdate( double delta ) 
{
	ScenePtr scene = getSceneManager();
	scene->update();

	if( runLoop )
	{
		c.r += 0.0000001f / float(delta); c.r = (c.r > 1.0f) ? 0.0f : c.r;
		c.g += 0.0000003f / float(delta); c.b = (c.b > 1.0f) ? 0.0f : c.b;
		c.b += 0.0000007f / float(delta); c.g = (c.g > 1.0f) ? 0.0f : c.g;
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
	if( keyEvent.keyCode == Keys::Space )
		debug( "time: %f", frameTimer.getElapsedTime
		() );

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

	if( keyEvent.keyCode == Keys::P )
	{
		if( sound->isPlaying() )
			sound->pause();
		else
			sound->play();
	}
}

//-----------------------------------//

void Example::onButtonPressed( const MouseButtonEvent& btnEvent )
{
	if( btnEvent.button == MouseButton::Right )
		runLoop = !runLoop;
}

//-----------------------------------//