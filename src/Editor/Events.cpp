/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "Editor.h"
#include "EditorInputManager.h"
#include "Tool.h"

namespace vapor { namespace editor {

//-----------------------------------//
// Event handlers
//-----------------------------------//

void EditorFrame::RefreshViewport()
{
	if( viewframe )
		viewframe->flagRedraw();
}

//-----------------------------------//

void EditorFrame::onRender()
{
	Viewport* viewport = viewframe->getViewport();
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
}

//-----------------------------------//

void EditorFrame::OnNodeSelected(wxTreeItemId old, wxTreeItemId id)
{
	const NodePtr& nodeOld = sceneTreeCtrl->getEntity( old );
	const NodePtr& nodeNew = sceneTreeCtrl->getEntity( id );
	
	if( !nodeNew )
		return;

	if( currentMode )
		currentMode->onNodeSelected( nodeOld, nodeNew );
}

//-----------------------------------//

void EditorFrame::handleUndoRedoOperation(Operations& firstOperations,
							 Operations& secondOperations, bool undo)
{
	if( firstOperations.empty() )
		return;

	Operation* op = firstOperations.back();

	if(!op)
		return;

	firstOperations.pop_back();
	secondOperations.push_back(op);

	if(undo)
		op->undo();
	else
		op->redo();

	updateUndoRedoUI();
	RefreshViewport();
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
		handleUndoRedoOperation(undoOperations, redoOperations, true);
		return;
	}
	//-----------------------------------//
	case Toolbar_Redo:
	{
		handleUndoRedoOperation(redoOperations, undoOperations, false);
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
		
		if( grid )
			grid->setVisible( !grid->isVisible() );
		
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
		wxSize newSize = viewframe->GetClientSize();
		const wxSize& nbSize = notebookCtrl->GetClientSize();

		if( notebookCtrl->IsShown() )
		{
			notebookCtrl->Hide();
		}
		else
		{
			notebookCtrl->Show();
			newSize.SetWidth( newSize.GetWidth() + nbSize.GetWidth() );
		}

		SetClientSize( newSize );
		Layout();
		
		return;
	}
	} // end switch
}

//-----------------------------------//

void EditorFrame::onModeSwitch( Tool* const newMode, int id )
{
	if( !newMode )
		return;

	if( currentMode )
		currentMode->onToolDisable();
	
	RefreshViewport();
	
	currentMode = newMode;
	currentMode->onToolEnable( id );
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
	bool undoEmpty = undoOperations.empty();
	toolBar->EnableTool( Toolbar_Undo, !undoEmpty );
		
	bool redoEmpty = redoOperations.empty();
	toolBar->EnableTool( Toolbar_Redo, !redoEmpty );
}

//-----------------------------------//

void EditorFrame::createEditorScene()
{
	// Create a nice grid for the editor.
	NodePtr grid( new Node("Grid") );
	grid->addTransform();
	grid->addComponent( GridPtr( new Grid() ) );
	grid->setTag( Tags::NonPickable, true );
	editorScene->add( grid );
}

//-----------------------------------//

void EditorFrame::createScene()
{
	const ScenePtr& scene = engine->getSceneManager();
	ResourceManager* const rm = engine->getResourceManager();

	rm->loadResource("Diffuse.glsl");
	rm->loadResource("Tex.glsl");
	rm->loadResource("Toon.glsl");
	rm->loadResource("Tex_Toon.glsl");
	rm->loadResource("Sky.glsl");
	rm->loadResource("Water.glsl");

	//MaterialPtr matSun( new Material("SunBlend") );
	//matSun->setProgram( "tex" );
	//matSun->setTexture( 0, "moon.png" );
	//matSun->setBlending( BlendingSource::SourceAlpha, BlendingDestination::OneMinusSourceAlpha );
	//matSun->setBackfaceCulling( false );
	//
	//RenderablePtr sunQuad( new Quad( 100.0f, 100.0f ) );
	//sunQuad->setMaterial( matSun );

	//GeometryPtr geom( new Geometry() );
	//geom->addRenderable( sunQuad, RenderGroup::Transparency );

	//NodePtr sun( new Node("Sun") );
	//sun->addTransform();
	//sun->addComponent( geom );
	//sun->addComponent( BillboardPtr( new Billboard( viewframe->getCamera() ) ) );
	//scene->add( sun );

	// Sky.
	SkydomePtr skydome( new Skydome() );
	//skydome->setSunNode( sun );
	
	const ImagePtr& clouds = rm->loadResource<Image>( "noise2.png" );
	skydome->setClouds( clouds );

	NodePtr sky( new Node("Sky") );
	sky->addTransform();	
	sky->addComponent( skydome );
	scene->add( sky );

	// Water.
	MaterialPtr matWater( new Material("WaterMat", "Water") );
	matWater->setTexture( 0, "water.png" );
	matWater->setBlending(BlendingSource::SourceAlpha, BlendingDestination::InverseSourceAlpha);
	WaterPtr water( new Water(matWater) );

	NodePtr nodeWater( new Node("Water") );
	nodeWater->addTransform();	
	nodeWater->addComponent( water );
	scene->add( nodeWater );

	TransformPtr transWater = nodeWater->getTransform();
	transWater->rotate(90.0f, 0.0f, 0.0f);
	transWater->scale(10.0f);
	transWater->translate(0.0f, 5.0f, 0.0f);

	// Light.
	light.reset( new Light( LightType::Directional ) );
	light->setDiffuseColor( Color::Red );
	light->setAmbientColor( Color::Yellow );

	TransformPtr transLight( new Transform(0.0f, 100.0f, 0.0f) );
	transLight->rotate(45.0f, 0.0f, 0.0f);

	NodePtr nodeLight( new Node("Light") );
	nodeLight->addComponent( transLight );
	nodeLight->addComponent( light );
	scene->add( nodeLight );

	// Terrain.
	MaterialPtr cellMaterial( new Material("CellMaterial") );
	cellMaterial->setTexture( 0, "PineTrunk.png" );
	cellMaterial->setProgram( "tex_toon" );

	TerrainSettings settings;
	settings.CellSize = 512;
	settings.NumberTiles = 32;
	settings.MaxHeight = 100;
	settings.Material = cellMaterial;

	TerrainPtr terrain( new Terrain(settings) );

	NodePtr terreno( new Node("Terreno") );
	terreno->addTransform();
	terreno->addComponent( terrain );
	scene->add( terreno );

	const ImagePtr& heightMap = rm->loadResource<Image>("height2.png");
	terrain->addCell( heightMap, 0, 0 );

	scene->update( 0.1f );
}

//-----------------------------------//

void EditorFrame::onMouseMove( const MouseMoveEvent& mve )
{
	if( !currentMode )
		return;
	
	currentMode->onMouseMove( mve );
}

//-----------------------------------//

void EditorFrame::onMouseDrag( const MouseDragEvent& mde )
{
	if( !currentMode )
		return;
	
	currentMode->onMouseDrag( mde );
}

//-----------------------------------//

void EditorFrame::onMousePress( const MouseButtonEvent& mbe )
{
	if( !currentMode )
		return;
	
	currentMode->onMouseButtonPress( mbe );
}

//-----------------------------------//

void EditorFrame::onMouseRelease( const MouseButtonEvent& mbe )
{
	if( !currentMode )
		return;
	
	currentMode->onMouseButtonRelease( mbe );
}

//-----------------------------------//

void EditorFrame::onMouseEnter()
{
	if( !currentMode )
		return;
	
	currentMode->onMouseEnter();
}

//-----------------------------------//

void EditorFrame::onMouseLeave()
{
	if( !currentMode )
		return;
	
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