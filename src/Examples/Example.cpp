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
	: Framework("Example", argv), runLoop( false ), c( 0.0f, 0.10f, 0.25f )
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
	//snd = rm->loadResource< resources::Sound >( "media/stereo.ogg" );

	ResourcePtr mesh = rm->loadResource( "media/cubo.ms3d" );

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

	//sound.reset( new scene::Sound( ls, snd) );
	//grp->add( sound );
	scene->add( grp );

	// Create a new Camera and position it to look at origin
	cam.reset( new Camera( getRenderDevice() ) );
	cam->translate( Vector3( 0.0f, 0.0f, -3.0f ) );
	cam->lookAt( Vector3::Zero );
	scene->add( cam );

	buildQuad();
}

//-----------------------------------//

void Example::buildQuad()
{
	ResourceManager* rm = getResourceManager();

	ProgramPtr program( new GLSL_Program( 
			rm->loadResource< GLSL_Shader >( "media/shader.vs" ),
			rm->loadResource< GLSL_Shader >( "media/shader.fs" ) ) );

	// Assign the renderable a material with a custom texture
	MaterialPtr mat( new Material( "SimpleMat", program ) );
	mat->addTexture( 0, tex );

	IndexBufferPtr ib( new IndexBuffer() );
	
	std::vector< short > ind;
	ind.push_back( 0 );
	ind.push_back( 1 );
	ind.push_back( 2 );
	ind.push_back( 3 );

	ib->set( ind );

	RenderablePtr quad( new Quad( mat, ib ) );
	GeometryPtr geom( new Geometry( quad ) );
	
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
		debug( "time: %f", frameTimer.getElapsedTime() );

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