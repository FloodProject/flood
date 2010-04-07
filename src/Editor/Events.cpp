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

void EditorFrame::RefreshViewport()
{
	viewport->getControl()->flagRedraw();
}

//-----------------------------------//

void EditorFrame::onRender()
{
	render::Device* const device = engine->getRenderDevice();
	
	const math::Color bg( 0.0f, 0.10f, 0.25f );
	device->setClearColor(bg);
	device->clearTarget();

	const CameraPtr& camera = viewport->getCamera();

	camera->render( editorScene );
	camera->render( engine->getSceneManager() );
}

//-----------------------------------//

void EditorFrame::onUpdate( double delta )
{
	if( !editorScene ) return;
	editorScene->update( delta );
	
	//const ScenePtr& scene = engine->getSceneManager();
	//if( !scene ) return;
	engine->update( delta );
}

//-----------------------------------//

void EditorFrame::onTaskEvent( const TaskEvent& te )
{
	//switch(te.event)
	//{
	//case TaskEvent::Added:
	//	debug( "Task added" );
	//	break;
	//case TaskEvent::Started:
	//	debug( "Task started" );
	//	break;
	//case TaskEvent::Finished:
	//	debug( "Task finished" );
	//	break;
	//}
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
		const NodePtr& grid = editorScene->getEntity( "Grid" );
		
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
	//-----------------------------------//
	case Toolbar_Undo:
	{
		if( undoOperations.empty() )
			return;

		Operation* op = undoOperations.back();
		undoOperations.pop_back();
		redoOperations.push_back(op);

		if( op ) op->undo();

		updateUndoRedoUI();
		RefreshViewport();
		return;
	}
	//-----------------------------------//
	case Toolbar_Redo:
	{
		if( redoOperations.empty() )
			return;

		Operation* op = redoOperations.back();
		redoOperations.pop_back();
		undoOperations.push_back(op);

		if( op ) op->redo();

		updateUndoRedoUI();
		RefreshViewport();
		return;
	}
	//-----------------------------------//
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
		return;
	}
	//-----------------------------------//
	case Toolbar_ToogleConsole:
	{
		codeEvaluator->Show( !codeEvaluator->IsShown() );
		codeEvaluator->SetFocus();
		return;
	}
	//-----------------------------------//
	case Toolbar_ToogleGrid:
	{
		const NodePtr& grid = editorScene->getEntity( "Grid" );
		if( grid ) grid->setVisible( !grid->isVisible() );
		RefreshViewport();
		return;
	}
	//-----------------------------------//
	class LogTask : public Task
	{
		void run()
		{
			debug("Task is logging!");
		}
	};
	case Toolbar_TooglePlay:
	{
		TaskManager* tm = engine->getTaskManager();
		tm->addTask( TaskPtr( new LogTask() ) );
		// Enable all simulations.
		//physics::PhysicsManager* pm = engine->getPhysicsManager();
		//if( pm ) pm->setSimulationEnabled( !pm->getSimulationEnabled() );
		return;
	}
	//-----------------------------------//
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
		
		return;
	}
	//-----------------------------------//
	} // end switch
}

//-----------------------------------//

void EditorFrame::onModeSwitch( Mode* const newMode, int id )
{
	if( !newMode ) return;

	if( currentMode )
		currentMode->onModeExit();
	
	RefreshViewport();
	
	currentMode = newMode;
	currentMode->onModeEnter( id );
}

//-----------------------------------//

void EditorFrame::registerOperation( Operation* const op )
{
	undoOperations.push_back( op );

	foreach( Operation* const op, redoOperations )
		delete op;

	redoOperations.clear();

	updateUndoRedoUI();
}

//-----------------------------------//

void EditorFrame::updateUndoRedoUI()
{
	bool u = undoOperations.empty();
	toolBar->EnableTool( Toolbar_Undo, u ? false : true );
		
	bool r = redoOperations.empty();
	toolBar->EnableTool( Toolbar_Redo, r ? false : true );
}

//-----------------------------------//

void EditorFrame::createEditorScene()
{
	// Create a scene node with editor stuff only.
	editorScene.reset( new Scene() );

	// Get the main viewport camera and add it to the scene.
	const NodePtr& camera = viewport->getCamera()->getNode();
	camera->setTag( Tags::NonPickable, true );
	editorScene->add( camera );

	camera->getTransform()->translate( 0.0f, 20.0f, -65.0f );

	// Create a nice grid for the editor.
	MaterialPtr mat( new Material("GridMaterial") );
	NodePtr grid( new Node("Grid") );
	grid->addComponent( TransformPtr( new Transform() ) );
	grid->addComponent( ComponentPtr( new Grid( mat ) ) );
	grid->setTag( Tags::NonPickable, true );
	editorScene->add( grid );

	// Update at least once before rendering.
	onUpdate( 0.0f );
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
	sun->addComponent( BillboardPtr( new Billboard( viewport->getCamera() ) ) );
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

	const ImagePtr& heightmap = rm->loadResource<Image>( "height2.png" );
	terrain->addCell( heightmap, 0, 0 );

	scene->update( 0.1f );
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