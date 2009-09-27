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
}

//-----------------------------------//

void Example::onSetupScene() 
{
	ScenePtr scene = getSceneManager();
	ResourceManager* rm = getResourceManager();

	GroupPtr grp( new Group() );

	ListenerPtr ls( new Listener( getAudioDevice() ) );
	ls->makeCurrent();
	grp->add( ls );

	//sound.reset( new scene::Sound( ls, snd) );
	//grp->add( sound );
	scene->add( grp );

	// Create a new Camera and position it to look at origin
	cam.reset( new Camera( getRenderDevice() ) );
	cam->translate( Vector3( 0.0f, 0.0f, -10.0f ) );
	cam->lookAt( Vector3::Zero );
	scene->add( cam );

	ProgramPtr program( new GLSL_Program( 
			rm->loadResource< GLSL_Shader >( "media/shader.vs" ),
			rm->loadResource< GLSL_Shader >( "media/shader.fs" ) ) );

	// Assign the renderable a material with a custom texture
	MaterialPtr mat( new Material( "SimpleMat", program ) );
	mat->addTexture( 0, "triton.png" );

	RenderablePtr rend ( new Quad( mat ) );
	GeometryPtr geom( new Geometry( rend ) );
	//scene->add( geom );

	MS3DPtr mesh = rm->getResource< MS3D >( "media/cubo.ms3d" );
	
	foreach( const RenderablePtr& rend, mesh->getRenderables() )
	{
		rend->getMaterial()->setProgram( program );
	}

	scene->add( mesh );
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