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
	
	ImagePtr img = rm->loadResource< Image >( "triton.png" );
	snd = rm->loadResource< resources::Sound >( "stereo.ogg" );
}

//-----------------------------------//

void Example::onSetupScene() 
{
	ScenePtr scene = getSceneManager();
	ResourceManager* rm = getResourceManager();

	// Create a new Camera and position it to look at origin
	cam.reset( new FirstPersonCamera( getInputManager(), getRenderDevice() ) );
	cam->translate( Vector3( 0.0f, 0.0f, 0.0f ) );
	//cam->lookAt( Vector3::Zero );
	scene->add( cam );

	ProgramPtr program( new GLSL_Program( 
			rm->loadResource< GLSL_Shader >( "shader.vs" ),
			rm->loadResource< GLSL_Shader >( "shader.fs" ) ) );

	mesh = rm->loadResource< MS3D >( "media/terreno.ms3d" );

	foreach( const RenderablePtr& rend, mesh->getRenderables() )
	{
		rend->getMaterial()->setProgram( program );
	}

	ListenerPtr ls( new Listener( getAudioDevice() ) );
	//sound.reset( new scene::Sound( ls, snd ) );
	//scene->add( ls ); scene->add( sound );

	scene->add( mesh );

	MaterialPtr mat( new Material( "SimpleWhite", program ) );
	FontPtr font = rm->loadResource< Font >( "Calibri.font" );
	LabelPtr label( new Label("Yey", font, mat) );

	scene->add( label );
}

//-----------------------------------//

void Example::onUpdate( double delta ) 
{
	ScenePtr scene = getSceneManager();
	scene->update( delta );

	if( runLoop )
	{
		c.r += 0.000001f / float(delta); c.r = (c.r > 1.0f) ? 0.0f : c.r;
		c.g += 0.000003f / float(delta); c.b = (c.b > 1.0f) ? 0.0f : c.b;
		c.b += 0.000007f / float(delta); c.g = (c.g > 1.0f) ? 0.0f : c.g;
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