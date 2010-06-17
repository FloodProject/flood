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
	: Framework("Example", argv)
{ }

//-----------------------------------//

void Example::onInit()
{
	camera.reset( new FirstPersonCamera( getInputManager(), getRenderDevice() ) );

	PageManager* pageManager = new PageManager( 512, camera );
	pageManager->onPageLoading += fd::bind(&Example::onPageLoading, this);

	addSubsystem(pageManager);
}

//-----------------------------------//

void Example::onPageLoading(const PageEvent& event)
{
	debug("%d,%d", event.pos.x, event.pos.y );

	ResourceManagerPtr rm = getResourceManager();
	const ImagePtr& heightMap = rm->loadResource<Image>( "height4.png" );
	terrain->addCell( heightMap, event.pos.x, event.pos.y );
}

//-----------------------------------//

void Example::onSetupResources() 
{
	ResourceManagerPtr rm = getResourceManager();
	
	rm->loadResource("Diffuse.glsl");
	rm->loadResource("Tex.glsl");
	rm->loadResource("Toon.glsl");
	rm->loadResource("Tex_Toon.glsl");
}

//-----------------------------------//

void Example::onSetupScene() 
{
	ScenePtr scene = getSceneManager();
	ResourceManagerPtr const rm = getResourceManager();
	RenderDevicePtr const rd = getRenderDevice();

	// Create a new Camera
	NodePtr nodeCamera( new Node( "MainCamera" ) );
	nodeCamera->addComponent( TransformPtr( new Transform( 0.0f, 20.0f, -65.0f ) ) );
	nodeCamera->addComponent( camera );
	scene->add( nodeCamera );

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

	LightPtr light( new Light( LightType::Directional ) );
	light->setDiffuseColor( Color::Red );
	light->setAmbientColor( Color::Yellow );
	
	NodePtr nodeLight( new Node("Light") );
	nodeLight->addTransform();
	nodeLight->addComponent( light );
	scene->add( nodeLight );

	MaterialPtr materialCell( new Material("CellMaterial") );
	materialCell->setTexture( 0, "dirt.png" );
	materialCell->setProgram( "tex_toon" );

	TerrainSettings settings;
	settings.CellSize = 512;
	settings.MaxHeight = 100;
	settings.Material = materialCell;

	terrain.reset( new Terrain(settings) );

	NodePtr nodeTerrain( new Node("Terreno") );
	nodeTerrain->addTransform();
	nodeTerrain->addComponent( terrain );
	scene->add( nodeTerrain );

	window = rd->getRenderWindow();
	window->makeCurrent();
	viewport = window->addViewport(camera);
	viewport->setClearColor( Color(0.0f, 0.10f, 0.25f) );
}

//-----------------------------------//
 
void Example::onUpdate( double delta ) 
{
	static double deltaPassed = 0.0f;
	
	deltaPassed += delta;
	bool clamped = clamp<double>(deltaPassed, 0, 1);

	if(!clamped)
		return;

	double fps = frameStats.getLastFPS();
	std::string newLabel = String::format("FPS: %d", int(fps));

	label->setText(newLabel);
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
		Log::showDebug = !Log::showDebug;

	if( keyEvent.keyCode == Keys::G )
	{
		fbo->bind();
		textureFBO->readImage()->save("depth.png");
		fbo->unbind();
	}

	if( keyEvent.keyCode == Keys::F )
		debug( "fps: %d", int(frameStats.getLastFPS()));

	if( keyEvent.keyCode == Keys::M )
	{
		debug( "min/avg/max: %f / %f / %f", 
					frameStats.minFrameTime,
					frameStats.avgFrameTime,
					frameStats.maxFrameTime );
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