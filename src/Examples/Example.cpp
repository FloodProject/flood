/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Example.h"
#include "Utilities.h"
#include "io/JsonSerializer.h"

//-----------------------------------//

Example::Example(const char** argv)
	: Framework("Example", argv)
{ }

//-----------------------------------//

void Example::onInit()
{
	camera.reset( new Camera() );
	camera->getFrustum().farPlane = 10000.0f;

	//PageManager* pageManager = new PageManager( 512, camera );
	//pageManager->onPageLoading += fd::bind(&Example::onPageLoading, this);

	//addSubsystem(pageManager);
}

//-----------------------------------//

void Example::onPageLoading( const PageEvent& event )
{
	LogDebug("%d,%d", event.pos.x, event.pos.y );

	ResourceManager* rm = getResourceManager();
	const ImagePtr& heightMap = rm->loadResource<Image>( "height4.png" );
	nodeTerrain->addCell( event.pos.x, event.pos.y, heightMap );
}

//-----------------------------------//

void Example::onSetupResources() 
{ }

//-----------------------------------//

void Example::onSetupScene() 
{
	ResourceManager* rm = getResourceManager();

	// Create a new Camera.
	EntityPtr nodeCamera( new Entity("MainCamera") );
	nodeCamera->addTransform();
	nodeCamera->addComponent( camera );
	nodeCamera->addComponent( CameraControllerPtr( new FirstPersonController() ) );
	nodeCamera->getTransform()->setPosition( Vector3(0, 20, -65) );
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

	//nodeFBO.reset( new Entity( "FBOquad" ) );
	//nodeFBO->addTransform();
	//nodeFBO->addComponent( GeometryPtr( new Geometry(quad) ) );
	//scene->add( nodeFBO );

	//MeshPtr meshSword = rm->loadResource<Mesh>("sword.ms3d");
	//EntityPtr nodeSword( new Entity("sword") );
	//nodeSword->addTransform();
	//nodeSword->addComponent( ModelPtr( new Model(meshSword) ) );
	//scene->add(nodeSword);

	//MeshPtr meshCT = rm->loadResource<Mesh>("ninja.ms3d", false);
	//ModelPtr modelCT( new Model(meshCT) );
	//modelCT->setAttachment("Joint17", nodeSword);
	//modelCT->setAnimation("Idle1");

	//EntityPtr nodeCT( new Entity("ct") );
	//nodeCT->addTransform();	
	//nodeCT->addComponent( modelCT );
	//scene->add(nodeCT);

#if 0
	labelFPS.reset( new Label( "", "Verdana.font") );
	
	EntityPtr nodeFPS( new Entity("LabelFPS") );
	nodeFPS->addTransform();
	nodeFPS->getTransform()->setPosition( Vector3(300.0f, 300.0f, 0) );
	nodeFPS->addComponent( labelFPS );
	scene->add( nodeFPS );
#endif

	EntityPtr grid( new Entity("Grid") );
	grid->addTransform();
	grid->addComponent( GridPtr( new Grid() ) );
	scene->add( grid );

#if 0
	LightPtr light( new Light(LightType::Directional) );
	light->setDiffuseColor( Color::Red );
	light->setAmbientColor( Color::Yellow );
	
	EntityPtr nodeLight( new Entity("Light") );
	nodeLight->addTransform();
	nodeLight->addComponent( light );
	scene->add( nodeLight );
#endif

#if 0
	MaterialPtr materialCell( new Material("CellMaterial") );
	materialCell->setTexture( 0, "sand.png" );
	materialCell->setProgram( "VertexLit" );

	TerrainSettings settings;
	settings.CellSize = 512;
	settings.MaxHeight = 100;
	settings.Material = materialCell;

	nodeTerrain.reset( new Terrain("Terrain", settings) );
	scene->add( nodeTerrain );
#endif

	EntityPtr sky( new Entity("Sky") );
	sky->addTransform();	
	sky->addComponent( SkydomePtr( new Skydome() ) );
	scene->add( sky );

	window = getRenderDevice()->getRenderWindow();
	window->makeCurrent();

	view = window->createView();
	view->setCamera(camera);
	view->setClearColor( Color(0.0f, 0.10f, 0.25f) );
}

//-----------------------------------//
 
void Example::onUpdate( float delta ) 
{
#if 0
	static float deltaPassed = 0.0f;
	
	deltaPassed += delta;
	bool clamped = Math::clamp<float>(deltaPassed, 0, 1);

	if(!clamped)
		return;

	float fps = frameStats.getLastFPS();
	std::string newFPS = String::format("FPS: %d", int(fps));

	labelFPS->setText(newFPS);
#endif
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
	if( event.keyCode == Keys::Space )
	{
		LogDebug( "time: %lf", frameTimer.getElapsedTime() );
	}
	else if( event.keyCode == Keys::F )
	{
		LogDebug( "FPS: %d", (int) frameStats.getLastFPS() );
	}
	else if( event.keyCode == Keys::M )
	{
		LogDebug( "min/avg/max: %lf / %lf / %lf", 
					frameStats.minFrameTime,
					frameStats.avgFrameTime,
					frameStats.maxFrameTime );
	}
}

//-----------------------------------//

void Example::onButtonPressed( const MouseButtonEvent& event )
{
	LogDebug( "%hd,%hd", event.x, event.y );
}

//-----------------------------------//