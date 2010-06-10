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
	if( viewframe )
		viewframe->flagRedraw();
}

//-----------------------------------//

void EditorFrame::onRender()
{
	ViewportPtr viewport = viewframe->getViewport();

	const CameraPtr& camera = viewport->getCamera();
	camera->setViewport( viewport );
	
	camera->render( engine->getSceneManager() );
	camera->render( editorScene, false );

	//TexturePtr depth = engine->getRenderDevice()->getShadowTextures()[light];
	//ImagePtr depthImage = depth->readImage();
	//depthImage->save( "shadowDepth.png" );
}

//-----------------------------------//

void EditorFrame::onUpdate( double delta )
{
	editorScene->update( delta );
	engine->update( delta );

	//viewframe->getControl()->flagRedraw();
}

//-----------------------------------//

void EditorFrame::OnNodeSelected(wxTreeItemId old, wxTreeItemId id)
{
	const NodePtr& n_old = sceneTreeCtrl->getEntity( old );
	const NodePtr& n_new = sceneTreeCtrl->getEntity( id );
	
	if( !n_new )
		return;

	if( currentMode )
		currentMode->onNodeSelected( n_old, n_new );
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
		std::string fn( fc.GetPath() );
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
	case Toolbar_TooglePlay:
	{
		// Enable all simulations.
		//physics::PhysicsManager* pm = engine->getPhysicsManager();
		//if( pm ) pm->setSimulationEnabled( !pm->getSimulationEnabled() );
		return;
	}
	//-----------------------------------//
	case Toolbar_ToogleSidebar:
	{
		wxSize szNew = viewframe->GetClientSize();
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
	// Create a nice grid for the editor.
	NodePtr grid( new Node("Grid") );
	grid->addTransform();
	grid->addComponent( ComponentPtr( new Grid() ) );
	grid->setTag( Tags::NonPickable, true );
	editorScene->add( grid );
}

//-----------------------------------//

void EditorFrame::createScene()
{
	const ScenePtr& scene = engine->getSceneManager();
	ResourceManagerPtr const rm = engine->getResourceManager();

	rm->loadResource("Diffuse.glsl");
	rm->loadResource("Tex.glsl");
	rm->loadResource("Toon.glsl");
	rm->loadResource("Tex_Toon.glsl");

	MaterialPtr matSun( new Material("SunBlend") );
	matSun->setProgram( "tex" );
	matSun->setTexture( 0, "moon.png" );
	matSun->setBlending( BlendingSource::SourceAlpha, BlendingDestination::OneMinusSourceAlpha );
	matSun->setBackfaceCulling( false );
	
	RenderablePtr sunQuad( new Quad( 100.0f, 100.0f ) );
	sunQuad->setMaterial( matSun );

	GeometryPtr geom( new Geometry() );
	geom->addRenderable( sunQuad, RenderGroup::Transparency );

	NodePtr sun( new Node("Sun") );
	sun->addTransform();
	sun->addComponent( geom );
	sun->addComponent( BillboardPtr( new Billboard( viewframe->getCamera() ) ) );
	scene->add( sun );

	SkydomePtr skydome( new Skydome() );
	skydome->setSunNode( sun );

	NodePtr sky( new Node("Sky") );
	sky->addTransform();	
	sky->addComponent( skydome );
	scene->add( sky );

	NodePtr lnode( new Node("Light") );
	TransformPtr lt( new Transform(0.0f, 100.0f, 0.0f) );
	lt->rotate(45.0f, 0.0f, 0.0f);
	lnode->addComponent( lt );
	light.reset( new Light( LightType::Directional ) );
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

	TerrainPtr terrain( new Terrain( settings ) );

	NodePtr terreno( new Node( "Terreno" ) );
	terreno->addTransform();
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

void EditorFrame::OnKeyDown(wxKeyEvent& event)
{
	im->processKeyEvent(event, true);
}

//-----------------------------------//

void EditorFrame::OnKeyUp(wxKeyEvent& event)
{
	im->processKeyEvent(event, false);
}

//-----------------------------------//

void EditorFrame::OnMouseEvent(wxMouseEvent& event)
{
	im->processMouseEvent(event);
}

//-----------------------------------//

} } // end namespaces