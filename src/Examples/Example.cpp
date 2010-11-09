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
	camera.reset( new Camera(renderDevice) );

	PageManager* pageManager = new PageManager( 512, camera );
	pageManager->onPageLoading += fd::bind(&Example::onPageLoading, this);

	addSubsystem(pageManager);
}

//-----------------------------------//

void Example::onPageLoading( const PageEvent& event )
{
	Log::debug("%d,%d", event.pos.x, event.pos.y );

	ResourceManager* rm = getResourceManager();
	const ImagePtr& heightMap = rm->loadResource<Image>( "height4.png" );
	nodeTerrain->addCell( event.pos.x, event.pos.y, heightMap );
}

//-----------------------------------//

void Example::onSetupResources() 
{
	ResourceManager* rm = getResourceManager();
	
	rm->loadResource("Diffuse.glsl");
	rm->loadResource("Tex.glsl");
	rm->loadResource("Toon.glsl");
	rm->loadResource("Tex_Toon.glsl");
	rm->loadResource("Tex_Toon_Skin.glsl");
	rm->loadResource("Sky.glsl");
	rm->loadResource("Water.glsl");
}

//-----------------------------------//

void Example::onSetupScene() 
{
	ResourceManager* rm = getResourceManager();

	// Create a new Camera.
	NodePtr nodeCamera( new Node("MainCamera") );
	nodeCamera->addComponent( TransformPtr( new Transform( 0.0f, 20.0f, -65.0f ) ) );
	nodeCamera->addComponent( camera );
	nodeCamera->addComponent( CameraControllerPtr( new FirstPersonController() ) );
	scene->add( nodeCamera );

	//bufferFBO = rd->createRenderBuffer( Settings() );
	////textureFBO = fbo->createRenderTexture( RenderBufferType::Color );
	//textureFBO = fbo->createRenderTexture( RenderBufferType::Depth );
	//fbo->check();
	//fbo->unbind();

	//viewportFBO = fbo->createView(camera);
	//viewportFBO->setClearColor( Color::Red );

	//MaterialPtr materialFBO( new Material( "FBO1", "Tex" ) );
	//materialFBO->setTexture( 0, textureFBO );

	//RenderablePtr quad( new Quad(100.0f, 100.0f) );
	//quad->setMaterial( materialFBO );

	//nodeFBO.reset( new Node( "FBOquad" ) );
	//nodeFBO->addTransform();
	//nodeFBO->addComponent( GeometryPtr( new Geometry(quad) ) );
	//scene->add( nodeFBO );

	MeshPtr meshSword = rm->loadResource<Mesh>("sword.ms3d");
	NodePtr nodeSword( new Node("sword") );
	nodeSword->addTransform();
	nodeSword->addComponent( ModelPtr( new Model(meshSword) ) );
	scene->add(nodeSword);

	MeshPtr meshCT = rm->loadResource<Mesh>("ninja.ms3d", false);
	ModelPtr modelCT( new Model(meshCT) );
	modelCT->attachNode("Joint17", nodeSword);
	modelCT->switchAnimation("Idle1");

	NodePtr nodeCT( new Node("ct") );
	nodeCT->addTransform();	
	nodeCT->addComponent( modelCT );
	scene->add(nodeCT);

	labelFPS.reset( new Label( "", "Verdana.font") );
	
	NodePtr nodeFPS( new Node("LabelFPS") );
	nodeFPS->addTransform();
	nodeFPS->addComponent( labelFPS );
	scene->add( nodeFPS );

	NodePtr grid( new Node("Grid") );
	grid->addComponent( TransformPtr( new Transform(0.0f, 1.0f, 0.0f) ) );
	grid->addComponent( GridPtr( new Grid() ) );
	scene->add( grid );

	LightPtr light( new Light(LightType::Directional) );
	light->setDiffuseColor( Color::Red );
	light->setAmbientColor( Color::Yellow );
	
	NodePtr nodeLight( new Node("Light") );
	nodeLight->addTransform();
	nodeLight->addComponent( light );
	scene->add( nodeLight );

	MaterialPtr materialCell( new Material("CellMaterial") );
	materialCell->setTexture( 0, "sand.png" );
	materialCell->setProgram( "tex_toon" );

	TerrainSettings settings;
	settings.CellSize = 512;
	settings.MaxHeight = 100;
	settings.Material = materialCell;

	nodeTerrain.reset( new Terrain("Terreno", settings) );
	scene->add( nodeTerrain );

	//SkydomePtr skydome( new Skydome() );

	//const ImagePtr& clouds = rm->loadResource<Image>( "noise2.png" );
	//skydome->setClouds( clouds );

	//NodePtr sky( new Node("Sky") );
	//sky->addTransform();	
	//sky->addComponent( skydome );
	//scene->add( sky );

	window = getRenderDevice()->getRenderWindow();
	window->makeCurrent();

	view = window->createView();
	view->setCamera(camera);
	view->setClearColor( Color(0.0f, 0.10f, 0.25f) );
}

//-----------------------------------//
 
void Example::onUpdate( double delta ) 
{
	static double deltaPassed = 0.0f;
	
	deltaPassed += delta;
	bool clamped = Math::clamp<double>(deltaPassed, 0, 1);

	if(!clamped)
		return;

	double fps = frameStats.getLastFPS();
	std::string newFPS = String::format("FPS: %d", int(fps));

	labelFPS->setText(newFPS);
}

//-----------------------------------//

void Example::onRender()
{
	// Render into the FBO first
	//fbo->bind();
	//textureFBO->bind();
	//nodeFBO->setVisible(false);
	//viewport2->update();
	//nodeFBO->setVisible(true);
	//fbo->unbind();

	// Render the scene.
	view->update();
}

//-----------------------------------//

void Example::onKeyPressed( const KeyEvent& event )
{
	if( event.keyCode == Keys::N )
	{
		Noise noise;

		std::vector<float> fn;
		noise.generate(fn, 256, 256);

		foreach( float& n, fn )
			n = pow((n+1) / 2.0f, 2);

		std::vector<byte> bn;

		foreach( float& n, fn )
			bn.push_back( n*255 );

		Image image(256, 256, PixelFormat::Depth);
		image.setStatus( ResourceStatus::Loaded );
		image.setBuffer(bn);

		image.save("noise.png");
	}

	else if( event.keyCode == Keys::Space )
		Log::debug( "time: %lf", frameTimer.getElapsedTime() );

	else if( event.keyCode == Keys::Pause )
		Log::showDebug = !Log::showDebug;

	else if( event.keyCode == Keys::G )
	{
		bufferFBO->bind();
		textureFBO->readImage()->save("depth.png");
		bufferFBO->unbind();
	}

	else if( event.keyCode == Keys::F )
		Log::debug( "FPS: %d", (int) frameStats.getLastFPS() );

	else if( event.keyCode == Keys::M )
	{
		debug( "min/avg/max: %lf / %lf / %lf", 
					frameStats.minFrameTime,
					frameStats.avgFrameTime,
					frameStats.maxFrameTime );
	}

	//else if( event.keyCode == Keys::J )
	//{
	//	Json::Value sc;
	//	scene->serialize( sc );
	//	
	//	File file( "Example.scene", StreamMode::Write );
	//	file.write( sc.toStyledString() );
	//}
}

//-----------------------------------//

void Example::onButtonPressed( const MouseButtonEvent& event )
{
	Log::debug( "%hd,%hd", event.x, event.y );
}

//-----------------------------------//