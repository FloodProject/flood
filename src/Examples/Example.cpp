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
	: Framework("Example", argv), c( 0.0f, 0.10f, 0.25f ),
	fpsUpdateTime( 0.0f )
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

	{
		math::EulerAngles ang( math::degreeToRadian( 90.0 ), 0.0f, 0.0f );
		math::Vector3 vec( -math::Vector3::UnitZ );
		Vector3 ret = vec * ang.getOrientationMatrix();
		int i = 0;
	}

	{
		math::EulerAngles ang( math::degreeToRadian( 90.0 ), 0.0f, 0.0f );
		math::Vector3 vec( -math::Vector3::UnitZ );
		Vector3 ret = vec * ang.getOrientationMatrix();

		math::EulerAngles ang2( 0.0f, math::degreeToRadian( 90.0 ), 0.0f );
		ret *= ang2.getOrientationMatrix();

		math::EulerAngles ang3( math::degreeToRadian( -90.0 ), 0.0f, 0.0f );
		ret *= ang3.getOrientationMatrix();
		int i = 0;
	}

	{
		math::EulerAngles ang( 0.0f, math::degreeToRadian( 90.0 ), 0.0f );
		math::Vector3 vec( -math::Vector3::UnitZ );
		Vector3 ret = vec * ang.getOrientationMatrix();
		int i = 0;
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
	if( lastFrameTime == 0 ) return "FPS:";

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
	NodePtr fps( new Node( "FPSNode" ) );
	fps->addComponent( TransformPtr( new Transform() ) );
	fps->addComponent( label );
	fps->getTransform()->translate( -300.0f, 220.0f, 0.0f );
	scene->add( fps );
	
	// Create a new Camera
	NodePtr camera( new Node( "MainCamera" ) );
	cam.reset( new FirstPersonCamera( getInputManager(), getRenderDevice() ) );
	camera->addComponent( TransformPtr( new Transform() ) );
	camera->addComponent( cam );
	scene->add( camera );

	mesh = rm->loadResource< MS3D >( "media/terreno.ms3d" );

	foreach( const RenderablePtr& rend, mesh->getRenderables() )
	{
		rend->getMaterial()->setProgram( tex );
	}

	NodePtr terreno( new Node( "Terreno" ) );
	terreno->addComponent( TransformPtr( new Transform() ) );
	terreno->addComponent( mesh );
	terreno->getTransform()->scale( 0.3f );
	scene->add( terreno );

	//mesh = rm->loadResource< MS3D >( "media/terreno.ms3d" );

	//foreach( const RenderablePtr& rend, mesh->getRenderables() )
	//{
	//	rend->getMaterial()->setProgram( program );
	//}

	//scene->add( mesh );

	//GridPtr grid( new Grid( mat ) );
	//scene->add( grid );

	//foreach( const RenderablePtr& rend, grid->getRenderables() )
	//{
	//	rend->getMaterial()->setProgram( diffuse );
	//}

	//ListenerPtr ls( new Listener( getAudioDevice() ) );
	//sound.reset( new scene::Sound( ls, snd ) );
	//scene->add( ls ); scene->add( sound );
}

//-----------------------------------//
 
void Example::onUpdate( float delta ) 
{
	ScenePtr scene = getSceneManager();
	scene->update( delta );
	
	if( fpsUpdateTime <= 1.0f )
	{
		fpsUpdateTime += delta;
	}
	else
	{
		label->setText( getFPS( lastFrameTime ) );
		fpsUpdateTime = 0.0f;
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

	//if( keyEvent.keyCode == Keys::V )
		//mesh->scale( 1.1f );
}

//-----------------------------------//

void Example::onButtonPressed( const MouseButtonEvent& btnEvent )
{

}

//-----------------------------------//
