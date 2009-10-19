/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include <vapor/PCH.h>

#include "Example.h"

#include <boost/lexical_cast.hpp>

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

std::string getFPS( float lastFrameTime )
{
	if( lastFrameTime == 0 ) return "";

	std::string fps( boost::lexical_cast< std::string >( int( 1.0f / lastFrameTime ) ) );
	return "FPS: " + fps;
}

//-----------------------------------//

void Example::onSetupScene() 
{
	ScenePtr scene = getSceneManager();
	ResourceManager* rm = getResourceManager();

	ProgramPtr program( new GLSL_Program( 
			rm->loadResource< GLSL_Shader >( "shader.vs" ),
			rm->loadResource< GLSL_Shader >( "shader.fs" ) ) );

	MaterialPtr mat( new Material( "SimpleWhite", program ) );
	FontPtr font = rm->loadResource< Font >( "Calibri.font" );
	
	label.reset( new Label( getFPS( lastFrameTime ), font, mat ) );

	LabelPtr zero( new Label( 
		"vaporEngine (" VAPOR_ENGINE_VERSION ")" 
		"\nDeveloped by triton with <3",
		font, mat ) );

	zero->translate( 0.0f, -font->getGlyphSize().second, 0.0f );

	scene->add( label );
	label->scale( 0.2f );
	//scene->add( zero );
	
	// Create a new Camera and position it to look at origin
	cam.reset( new FirstPersonCamera( getInputManager(), getRenderDevice() ) );
	cam->translate( 0.0f, 0.0f, 0.0f );
	scene->add( cam );

	mesh = rm->loadResource< MS3D >( "media/ct.ms3d" );

	foreach( const RenderablePtr& rend, mesh->getRenderables() )
	{
		rend->getMaterial()->setProgram( program );
	}

	mesh->translate( 0.0f, 50.0f, 0.0f );
	mesh->scale( 0.3f );

	scene->add( mesh );

	mesh = rm->loadResource< MS3D >( "media/terreno.ms3d" );

	foreach( const RenderablePtr& rend, mesh->getRenderables() )
	{
		rend->getMaterial()->setProgram( program );
	}

	scene->add( mesh );

	ListenerPtr ls( new Listener( getAudioDevice() ) );
	//sound.reset( new scene::Sound( ls, snd ) );
	//scene->add( ls ); scene->add( sound );
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
	
	label->setText( getFPS( lastFrameTime ) );
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
		//debug( "fps: %d", int( 1.0f / lastFrameTime ) );

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

	if( keyEvent.keyCode == Keys::V )
		mesh->translate( 0.0f, 1.0f, 0.0f );
}

//-----------------------------------//

void Example::onButtonPressed( const MouseButtonEvent& btnEvent )
{
	if( btnEvent.button == MouseButton::Right )
		runLoop = !runLoop;
}

//-----------------------------------//