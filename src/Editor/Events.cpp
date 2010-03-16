/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "Editor.h"

namespace vapor { namespace editor {

//-----------------------------------//
// event handlers
//-----------------------------------//

void EditorFrame::RefreshCanvas()
{
	static int i = 0;
	debug( "Refresh %d", i++ );
	viewport->vaporCtrl->needsRedraw = true;
}

//-----------------------------------//

void EditorFrame::onRender()
{
	static int i = 0;
	debug( "Render %d", i++ );

	render::Device* const device = engine->getRenderDevice();
	
	const math::Color bg( 0.0f, 0.10f, 0.25f );
	device->setClearColor(bg);
	device->clearTarget();

	viewport->camera->render( editorScene );
	viewport->camera->render( engine->getSceneManager() );
}

//-----------------------------------//

void EditorFrame::onUpdate()
{
	if( !editorScene ) return;

	editorScene->update( 0.01f );
}

//-----------------------------------//

void EditorFrame::onCameraTransform()
{
	RefreshCanvas();
}

//-----------------------------------//

void EditorFrame::OnNodeSelected(wxTreeItemId old, wxTreeItemId id)
{
	const NodePtr& n_old = sceneTreeCtrl->getEntity( old );
	const NodePtr& n_new = sceneTreeCtrl->getEntity( id );
	
	if( currentMode )
		currentMode->onNodeSelected( n_old, n_new );
}

//-----------------------------------//

void EditorFrame::OnKeyDown(wxKeyEvent& event)
{
	if( event.GetKeyCode() == 'g' )
	{
		const NodePtr& grid = editorScene->getEntity( "EditorGrid" );
		if( !grid ) return;
		grid->setVisible( !grid->isVisible() );
	}
}

//-----------------------------------//

void EditorFrame::OnToolbarButtonClick(wxCommandEvent& event)
{
	const int id = event.GetId();

	// If the id is registered to a mode, then switch mode...
	if( modesMap.find(id) != modesMap.end() )
	{
		onModeSwitch( modesMap[id], id );
		return;
	}

	switch(id) 
	{
	
	case Toolbar_Undo:
	{
		if( operations.empty() )
			break;

		Operation* op = operations.top();
		operations.pop();

		assert( op != nullptr );
		if( op ) op->undo();

		if( operations.empty() )
			toolBar->EnableTool( Toolbar_Undo, false );

		break;
	}

	case Toolbar_Save:
	{
		// Ask for file name to save as.
		wxFileDialog fc( this, wxFileSelectorPromptStr, wxEmptyString,
			wxEmptyString, "Scene files (*.scene)|*.scene", wxFC_SAVE );
		
		if( fc.ShowModal() != wxID_OK )
			return;

		// Serialize scene to JSON.
		Json::Value scene;
		engine->getSceneManager()->serialize( scene );

		// Save it to a file.
		std::string fn( fc.GetFilename() );
		serializeToFile( scene, fn );
		break;
	}

	case Toolbar_ToogleConsole:
	{
		codeEvaluator->Show( !codeEvaluator->IsShown() );
		codeEvaluator->SetFocus();
		break;
	}
	
	case Toolbar_ToogleGrid:
	{
		const NodePtr& grid = editorScene->getEntity( "EditorGrid" );
		if( grid ) grid->setVisible( !grid->isVisible() );
		RefreshCanvas();
		break;
	}

	case Toolbar_TooglePlay:
	{
		//physics::PhysicsManager* pm = engine->getPhysicsManager();
		//if( pm ) pm->setSimulationEnabled( !pm->getSimulationEnabled() );
		break;
	}

	case Toolbar_ToogleSidebar:
	{
		wxSize szNew = viewport->GetClientSize();
		const wxSize& szNB = notebookCtrl->GetClientSize();

		if( notebookCtrl->IsShown() )
		{
			notebookCtrl->Hide();
		}
		else
		{
			notebookCtrl->Show();
			szNew.SetWidth( szNew.GetWidth() + szNB.GetWidth() );
		}

		SetClientSize( szNew );
		Layout();
	}

	} // end switch
}

//-----------------------------------//

void EditorFrame::onModeSwitch( Mode* newMode, int id )
{
	if( !newMode ) return;

	if( currentMode )
		currentMode->onModeExit();
	
	currentMode = newMode;
	currentMode->onModeEnter( id );
}

//-----------------------------------//

void EditorFrame::registerOperation( Operation* const op )
{
	operations.push( op );

	if( !toolBar->GetToolEnabled( Toolbar_Undo ) )
	{
		toolBar->EnableTool( Toolbar_Undo, true );
	}
}

//-----------------------------------//

void EditorFrame::createEditorScene()
{
	// Create a scene node with editor stuff only.
	editorScene.reset( new Scene() );

	// Get the main viewport camera and add it to the scene.
	const NodePtr& camera = viewport->cameraNode;
	camera->getTransform()->translate( 0.0f, 20.0f, -65.0f );
	editorScene->add( camera );

	// Create a nice grid for the editor.
	MaterialPtr mat( new Material("GridMaterial") );
	NodePtr grid( new Node("EditorGrid") );
	grid->addComponent( TransformPtr( new Transform() ) );
	grid->addComponent( ComponentPtr( new Grid( mat ) ) );
	grid->setTag( Tags::NonPickable, true );
	grid->setTag( EditorTags::EditorOnly, true );
	editorScene->add( grid );

	// Update at least once before rendering.
	onUpdate();
}

//-----------------------------------//

void EditorFrame::createScene()
{
	const ScenePtr& scene = engine->getSceneManager();
	ResourceManager* const rm = engine->getResourceManager();

	ProgramPtr diffuse( new GLSL_Program( 
			rm->loadResource< GLSL_Shader >( "diffuse.vs" ),
			rm->loadResource< GLSL_Shader >( "diffuse.fs" ) ) );

	ProgramPtr tex( new GLSL_Program( 
			rm->loadResource< GLSL_Shader >( "tex.vs" ),
			rm->loadResource< GLSL_Shader >( "tex.fs" ) ) );

	ProgramManager::getInstance().registerProgram( "diffuse", diffuse );
	ProgramManager::getInstance().registerProgram( "tex", tex );

	ProgramPtr toon( new GLSL_Program( 
			rm->loadResource< GLSL_Shader >( "toon.vs" ),
			rm->loadResource< GLSL_Shader >( "toon.fs" ) ) );

	ProgramManager::getInstance().registerProgram( "toon", toon );

	ProgramPtr tex_toon( new GLSL_Program( 
			rm->loadResource< GLSL_Shader >( "tex_toon.vs" ),
			rm->loadResource< GLSL_Shader >( "tex_toon.fs" ) ) );
	
	ProgramManager::getInstance().registerProgram( "tex_toon", tex_toon );
	
	MaterialPtr matSun( new Material("SunBlend") );
	matSun->setProgram( "tex" );
	matSun->setTexture( 0, "moon.png" );
	matSun->setBlending( BlendingOperationSource::SourceAlpha,
		BlendingOperationDestination::OneMinusSourceAlpha );
	matSun->setBackfaceCulling( false );
	
	RenderablePtr sunQuad( new render::Quad( 100.0f, 100.0f ) );
	sunQuad->setMaterial( matSun );

	GeometryPtr geom( new Geometry() );
	geom->addRenderable( sunQuad, RenderGroup::Transparency );

	NodePtr sun( new Node("Sun") );
	sun->addComponent( TransformPtr( new Transform() ) );
	sun->addComponent( geom );
	sun->addComponent( BillboardPtr( new Billboard( viewport->camera ) ) );
	scene->add( sun );

	MaterialPtr mat( new Material("SkyMaterial") );
	SkydomePtr skydome( new Skydome( mat ) );
	skydome->setSunNode( sun );

	NodePtr sky( new Node("Sky") );
	sky->addComponent( TransformPtr( new Transform() ) );	
	sky->addComponent( skydome );
	scene->add( sky );

	NodePtr lnode( new Node("Light") );
	lnode->addComponent( TransformPtr( new Transform() ) );
	LightPtr light( new Light( LightType::Point ) );
	light->diffuseColor = Colors::Red;
	light->ambientColor = Colors::Yellow;
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

	TerrainPtr terrain( new Terrain( settings ) );

	NodePtr terreno( new Node( "Terreno" ) );
	terreno->addComponent( TransformPtr( new Transform() ) );
	terreno->addComponent( terrain );
	scene->add( terreno );

	const ImagePtr& heightmap = rm->loadResource< Image >( "height2.png" );
	/*const CellPtr& cell = */terrain->createCell( heightmap, 0, 0 );
}

//-----------------------------------//

void EditorFrame::onMouseMove( const MouseMoveEvent& mve )
{
	if( !currentMode ) return;
	currentMode->onMouseMove( mve );
}

//-----------------------------------//

void EditorFrame::onMouseDrag( const MouseDragEvent& mde )
{
	if( !currentMode ) return;
	currentMode->onMouseDrag( mde );
}

//-----------------------------------//

void EditorFrame::onMousePress( const MouseButtonEvent& mbe )
{
	if( !currentMode ) return;
	currentMode->onMouseButtonPress( mbe );
}

//-----------------------------------//

void EditorFrame::onMouseRelease( const MouseButtonEvent& mbe )
{
	if( !currentMode ) return;
	currentMode->onMouseButtonRelease( mbe );
}

//-----------------------------------//

void EditorFrame::onMouseEnter()
{
	if( !currentMode ) return;
	currentMode->onMouseEnter();
}

//-----------------------------------//

void EditorFrame::onMouseLeave()
{
	if( !currentMode ) return;
	currentMode->onMouseLeave();
}

//-----------------------------------//

void EditorFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    // true forces the frame to close.
    Close(true);
}

//-----------------------------------//

} } // end namespaces