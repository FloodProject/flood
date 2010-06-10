/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Example.h"

//-----------------------------------//

Example::Example(const char** argv)
	: Framework("Example", argv), fpsUpdateTime( 0.0f )
{

}

//-----------------------------------//

void Example::onInit()
{
	//physicsManager->createWorld();
	getVFS()->mountDefaultLocations();
}

//-----------------------------------//

void Example::onSetupResources() 
{
	ResourceManagerPtr rm = getResourceManager();
	
	ImagePtr img = rm->loadResource< Image >( "triton.png" );
	snd = rm->loadResource< Sound >( "stereo.ogg" );

	rm->loadResource("Diffuse.glsl");
	rm->loadResource("Tex.glsl");
	rm->loadResource("Toon.glsl");
	rm->loadResource("Tex_Toon.glsl");
}

//-----------------------------------//

std::string getFPS( float lastFrameTime )
{
	std::string str = "FPS: ";
	if( lastFrameTime == 0 ) return str;
	std::string fps( num_to_str(int( 1.0f / lastFrameTime ) ) );
	return str + fps;
}

//-----------------------------------//

void Example::onSetupScene() 
{
	ScenePtr scene = getSceneManager();
	ResourceManagerPtr const rm = getResourceManager();
	render::DevicePtr const rd = getRenderDevice();

	// Create a new Camera
	NodePtr camera( new Node( "MainCamera" ) );
	cam.reset( new FirstPersonCamera( getInputManager(), getRenderDevice() ) );
	camera->addComponent( TransformPtr( new Transform( 0.0f, 20.0f, -65.0f ) ) );
	camera->addComponent( cam );
	scene->add( camera );

	//fbo = rd->createRenderBuffer( Settings() );
	////fbo_tex = fbo->createRenderTexture( RenderBufferType::Color );
	//fbo_tex = fbo->createRenderTexture( RenderBufferType::Depth );
	//fbo->check();
	//fbo->unbind();

	//viewport2 = fbo->addViewport(cam);
	//viewport2->setClearColor( Color::Red );

	//MaterialPtr fbo_mat( new Material( "FBO1", "Tex" ) );
	//fbo_mat->setTexture( 0, fbo_tex );

	//RenderablePtr quad( new Quad(100.0f, 100.0f) );
	//quad->setMaterial( fbo_mat );

	//fbo_node.reset( new Node( "FBOquad" ) );
	//fbo_node->addTransform();
	//fbo_node->addComponent( GeometryPtr( new Geometry(quad) ) );
	//scene->add( fbo_node );

	MeshPtr mesh = rm->loadResource<Mesh>("ct.ms3d");
	NodePtr ct( new Node("ct") );
	ct->addTransform();
	ct->addComponent( ModelPtr( new Model(mesh) ) );
	scene->add(ct);
	
	label.reset( new Label( "", "Verdana.font") );
	NodePtr fps( new Node("FPSNode") );
	fps->addTransform();
	fps->addComponent( label );
	scene->add( fps );

	NodePtr grid( new Node("Grid") );
	grid->addTransform();
	grid->addComponent( GridPtr( new Grid() ) );
	scene->add( grid );

	NodePtr lnode( new Node("Light") );
	lnode->addTransform();
	LightPtr light( new Light( LightType::Directional ) );
	light->setDiffuseColor( Color::Red );
	light->setAmbientColor( Color::Yellow );
	lnode->addComponent( light );
	scene->add( lnode );

	MaterialPtr cellMaterial( new Material("CellMaterial") );
	cellMaterial->setTexture( 0, "PineTrunk.png" );
	cellMaterial->setProgram( "tex_toon" );

	TerrainSettings settings;
	settings.CellSize = 512;
	settings.NumberTiles = 32;
	settings.MaxHeight = 100;
	settings.Material = cellMaterial;

	TerrainPtr terrain( new Terrain(settings) );

	const ImagePtr& heightmap = rm->loadResource<Image>( "height2.png" );
	terrain->addCell( heightmap, 0, 0 );

	NodePtr terreno( new Node( "Terreno" ) );
	terreno->addTransform();
	terreno->addComponent( terrain );
	scene->add( terreno );

	window = rd->getRenderWindow();
	window->makeCurrent();
	viewport = window->addViewport(cam);
	viewport->setClearColor( Color(0.0f, 0.10f, 0.25f) );
}

//-----------------------------------//
 
void Example::onUpdate( double delta ) 
{
	if( fpsUpdateTime <= 1.0f )
	{
		fpsUpdateTime += delta;
	}
	else
	{
		label->setText( getFPS(lastFrameTime) );
		fpsUpdateTime = 0.0f;
	}
}

//-----------------------------------//

void Example::onRender()
{
	//// Render into the FBO first
	//fbo->bind();
	//fbo_tex->bind();
	//fbo_node->setVisible(false);
	//viewport2->update();
	//fbo_node->setVisible(true);
	//fbo->unbind();

	// Render the scene
	viewport->update();
}

//-----------------------------------//

void Example::onKeyPressed( const KeyEvent& keyEvent )
{
	if( keyEvent.keyCode == Keys::Space )
		debug( "time: %f", frameTimer.getElapsedTime() );

	if( keyEvent.keyCode == Keys::Pause )
		showDebug = !showDebug;

	if( keyEvent.keyCode == Keys::G )
	{
		fbo->bind();
		fbo_tex->readImage()->save("depth.png");
		fbo->unbind();
	}

	if( keyEvent.keyCode == Keys::F )
		debug( "fps: %d", int( 1.0f / lastFrameTime ) );

	if( keyEvent.keyCode == Keys::M )
	{
		debug( "min/avg/max: %f / %f / %f", 
					minFrameTime, avgFrameTime, maxFrameTime );
	}

	if( keyEvent.keyCode == Keys::J )
	{
		Json::Value sc;
		getSceneManager()->serialize( sc );
		File file( "Example.scene", AccessMode::Write );
		file.write( sc.toStyledString() );
	}
}

//-----------------------------------//

void Example::onButtonPressed( const MouseButtonEvent& event )
{
	debug( "%hd,%hd", event.x, event.y );
}

//-----------------------------------//