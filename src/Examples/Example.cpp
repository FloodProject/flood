/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include <vapor/PCH.h>

#include "Example.h"
#include "vapor/scene/Grid.h"

#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>

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

	math::EulerAngles ang( math::degreeToRadian( 90.0 ), 0.0f, 0.0f );
	math::Vector3 vec( math::Vector3::UnitZ );

	Vector3 ret = vec * ang.getOrientationMatrix();
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

	ProgramPtr tex( new GLSL_Program( 
			rm->loadResource< GLSL_Shader >( "tex.vs" ),
			rm->loadResource< GLSL_Shader >( "tex.fs" ) ) );

	ProgramPtr diffuse( new GLSL_Program( 
			rm->loadResource< GLSL_Shader >( "diffuse.vs" ),
			rm->loadResource< GLSL_Shader >( "diffuse.fs" ) ) );

	MaterialPtr mat( new Material( "FontMaterial", tex ) );
	FontPtr font = rm->loadResource< Font >( "Verdana.font" );
	
	label.reset( new Label( getFPS( lastFrameTime ), font, mat ) );
	label->translate( -300.0f, 220.0f, 0.0f );
	scene->add( label );
	
	// Create a new Camera and position it to look at origin
	cam.reset( new FirstPersonCamera( getInputManager(), getRenderDevice() ) );
	cam->translate( 0.0f, 0.0f, 0.0f );
	scene->add( cam );

	mesh = rm->loadResource< MS3D >( "media/terreno.ms3d" );

	foreach( const RenderablePtr& rend, mesh->getRenderables() )
	{
		rend->getMaterial()->setProgram( tex );
	}

	//mesh->scale( 0.3f );

	scene->add( mesh );

	//mesh = rm->loadResource< MS3D >( "media/terreno.ms3d" );

	//foreach( const RenderablePtr& rend, mesh->getRenderables() )
	//{
	//	rend->getMaterial()->setProgram( program );
	//}

	//scene->add( mesh );

	GridPtr grid( new Grid( mat ) );
	scene->add( grid );

	foreach( const RenderablePtr& rend, grid->getRenderables() )
	{
		rend->getMaterial()->setProgram( diffuse );
	}

	ListenerPtr ls( new Listener( getAudioDevice() ) );
	sound.reset( new scene::Sound( ls, snd ) );
	scene->add( ls ); scene->add( sound );
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
		mesh->scale( 1.1f );
}

//-----------------------------------//

void Example::onButtonPressed( const MouseButtonEvent& btnEvent )
{
	if( btnEvent.button == MouseButton::Right )
		runLoop = !runLoop;
}

//-----------------------------------//