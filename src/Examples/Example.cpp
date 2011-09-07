/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Example.h"

int main()
{
	Example example;
	example.run();

	return EXIT_SUCCESS;
}

//-----------------------------------//

Example::Example(const char** argv) : Framework("Example")
{
}

//-----------------------------------//

Example::~Example()
{
	camera.reset();
	view = nullptr;
}

//-----------------------------------//

void Example::onInit()
{

}

//-----------------------------------//

void Example::onSetupResources() 
{

}

//-----------------------------------//

void Example::onSetupScene() 
{
	camera.reset( AllocateThis(Camera) );
	camera->getFrustum().farPlane = 10000.0f;

	Serializer* serializer = SerializerCreateJSON( AllocatorGetThis() );
	scene = (Scene*) SerializerLoadObjectFromFile(serializer, "Assets/Scenes/2guys1box.scene");
	Deallocate(serializer);

	// Create a new Camera.
	EntityPtr nodeCamera( AllocateThis(Entity, "MainCamera") );
	nodeCamera->addTransform();
	nodeCamera->addComponent( camera );
	nodeCamera->addComponent( AllocateThis(FirstPersonController) );
	nodeCamera->getTransform()->setPosition( Vector3(0, 20, -65) );
	scene->add( nodeCamera );

#ifdef ENABLE_FSP
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
#endif

#ifdef ENABLE_LIGHT
	LightPtr light( new Light(LightType::Directional) );
	light->setDiffuseColor( Color::Red );
	light->setAmbientColor( Color::Yellow );
	
	EntityPtr nodeLight( new Entity("Light") );
	nodeLight->addTransform();
	nodeLight->addComponent( light );
	scene->add( nodeLight );
#endif

#ifdef ENABLE_TERRAIN
	MaterialPtr materialCell( new Material("CellMaterial") );
	materialCell->setTexture( 0, "sand.png" );
	materialCell->setProgram( "VertexLit" );

	TerrainSettings settings;
	settings.CellSize = 512;
	settings.MaxHeight = 100;
	settings.Material = materialCell;

	EntityPtr nodeTerrain;
	nodeTerrain.reset( AllocateThis(Terrain, "Terrain", settings) );
	scene->add( nodeTerrain );
#endif

#ifdef ENABLE_SKY
	EntityPtr sky = AllocateThis(Entity, "Sky");
	sky->addTransform();	
	sky->addComponent( AllocateThis(Skydome) );
	scene->add( sky );
#endif

	view = window->createView();
	view->setCamera(camera);
	view->setClearColor( Color(0.0f, 0.10f, 0.25f) );
}

//-----------------------------------//
 
void Example::onUpdate( float delta ) 
{
#if ENABLE_FPS
	static float deltaPassed = 0.0f;
	
	deltaPassed += delta;
	bool clamped = Math::clamp<float>(deltaPassed, 0, 1);

	if(!clamped)
		return;

	float fps = frameStats.getLastFPS();
	std::string newFPS = String::format("FPS: %d", int(fps));

	labelFPS->setText(newFPS);
#endif

	scene->update(delta);
}

//-----------------------------------//

void Example::onRender()
{
	// Render the scene.
	view->update(scene);
}

//-----------------------------------//

void Example::onKeyPressed( const KeyEvent& event )
{
	if( event.keyCode == Keys::Space )
	{
		LogDebug( "time: %lf", TimerGetElapsed(&frameTimer) );
	}
#ifdef ENABLE_FPS
	else if( event.keyCode == Keys::F )
	{
		LogDebug( "FPS: %d", (int) frameStats.getLastFPS() );
	}
#endif
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
}