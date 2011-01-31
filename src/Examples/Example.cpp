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

class A
{
	DECLARE_CLASS_()
public:
	float f;
	void setF(float v) {}
};

BEGIN_CLASS(A)
	FIELD_PRIMITIVE_SETTER(float, f, F)
END_CLASS()

class B
{
	DECLARE_CLASS_()
public:
	float f;
	void setF(float v) {}
};

BEGIN_CLASS(B)
	FIELD_PRIMITIVE_SETTER(float, f, F)
END_CLASS()

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
	Log::debug("%d,%d", event.pos.x, event.pos.y );

	ResourceManager* rm = getResourceManager();
	const ImagePtr& heightMap = rm->loadResource<Image>( "height4.png" );
	nodeTerrain->addCell( event.pos.x, event.pos.y, heightMap );
}

//-----------------------------------//

void Example::onSetupResources() 
{
	ResourceManager* const rm = getResourceManager();

	std::vector<std::string> files = System::enumerateFiles("Media/Shaders");

	for(uint i = 0; i < files.size(); i++ )
		rm->loadResource(files[i]);
}

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

	labelFPS.reset( new Label( "", "Verdana.font") );
	
	EntityPtr nodeFPS( new Entity("LabelFPS") );
	nodeFPS->addTransform();
	nodeFPS->getTransform()->setPosition( Vector3(300.0f, 300.0f, 0) );
	nodeFPS->addComponent( labelFPS );
	scene->add( nodeFPS );

	EntityPtr grid( new Entity("Grid") );
	grid->addTransform();
	grid->addComponent( GridPtr( new Grid() ) );
	scene->add( grid );

	LightPtr light( new Light(LightType::Directional) );
	light->setDiffuseColor( Color::Red );
	light->setAmbientColor( Color::Yellow );
	
	EntityPtr nodeLight( new Entity("Light") );
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

	nodeTerrain.reset( new Terrain("Terrain", settings) );
	scene->add( nodeTerrain );

	EntityPtr sky( new Entity("Sky") );
	sky->addTransform();	
	sky->addComponent( SkydomePtr( new Skydome() ) );
	scene->add( sky );

	window = getRenderDevice()->getRenderWindow();
	window->makeCurrent();

	view = window->createView();
	view->setCamera(camera);
	view->setClearColor( Color(0.0f, 0.10f, 0.25f) );

	//Object object;
	//object.instance = scene.get();
	//object.type = &Scene::getStaticType();

	FileStream stream("Test.scene", StreamMode::Write);
	stream.open();

	//JsonSerializer json(stream);
	//scene->save(serializer);
	
	//ObjectSerializer serializer(json);
	//serializer.save();
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

		for( uint i = 0; i < fn.size(); i++ )
		{
			float& n = fn[i];
			n = pow((n+1) / 2.0f, 2);
		}

		std::vector<byte> bn;
		
		for( uint i = 0; i < fn.size(); i++ )
			bn.push_back( fn[i]*255 );

		Image image(256, 256, PixelFormat::Depth);
		image.setStatus( ResourceStatus::Loaded );
		image.setBuffer(bn);

		image.save("noise.png");
	}
	else if( event.keyCode == Keys::Space )
	{
		Log::debug( "time: %lf", frameTimer.getElapsedTime() );
	}
	else if( event.keyCode == Keys::G )
	{
		bufferFBO->bind();
		textureFBO->readImage()->save("depth.png");
		bufferFBO->unbind();
	}
	else if( event.keyCode == Keys::F )
	{
		Log::debug( "FPS: %d", (int) frameStats.getLastFPS() );
	}
	else if( event.keyCode == Keys::M )
	{
		Log::debug( "min/avg/max: %lf / %lf / %lf", 
					frameStats.minFrameTime,
					frameStats.avgFrameTime,
					frameStats.maxFrameTime );
	}
}

//-----------------------------------//

void Example::onButtonPressed( const MouseButtonEvent& event )
{
	Log::debug( "%hd,%hd", event.x, event.y );
}

//-----------------------------------//