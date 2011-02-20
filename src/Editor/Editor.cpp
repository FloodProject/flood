/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "Editor.h"
#include "Utilities.h"
#include "EditorIcons.h"
#include "RenderControl.h"
#include "PluginManager.h"
#include "PluginManagerFrame.h"
#include "UndoManager.h"
#include "Viewframe.h"
#include "EditorInputManager.h"
#include "Events.h"
#include "EditorTags.h"
#include "ResourceDrop.h"
#include "wxFourWaySplitter.h"
#include "StackWalker.h"
#include "Core/FileSystem.h"

// Editor plugins
#include "plugins/Project/ProjectPlugin.h"
#include "plugins/UndoRedo/UndoPlugin.h"
#include "plugins/Scene/ScenePlugin.h"
#include "plugins/Log/LogPlugin.h"
#include "plugins/Console/ConsolePlugin.h"
#include "plugins/Resources/ResourcesPlugin.h"
#include "plugins/Property/PropertyPlugin.h"
#include "plugins/Selection/SelectionPlugin.h"
#include "plugins/Gizmos/GizmoPlugin.h"
#include "plugins/Terrain/TerrainPlugin.h"
#include "plugins/Camera/CameraPlugin.h"
#include "plugins/Sample/SamplePlugin.h"

#include <wx/debugrpt.h>

wxIMPLEMENT_WXWIN_MAIN_CONSOLE

namespace vapor { namespace editor {

//-----------------------------------//

wxIMPLEMENT_APP_NO_MAIN(EditorApp);

bool EditorApp::OnInit()
{
	wxHandleFatalExceptions();

    if( !wxApp::OnInit() ) return false;

	wxImage::AddHandler( new wxPNGHandler() );

    EditorFrame* frame = new EditorFrame(VAPOR_EDITOR_NAME);
	frame->SetSize(800, 500);
    frame->Show(true);
	
	SetTopWindow(frame);

    return true;
}

//-----------------------------------//

void EditorApp::OnFatalException()
{
	//StackWalkerLog sw;
	//sw.ShowCallstack();
	//sw.Log();

	//wxMessageOutputBest output;
	//output.Output("An exception occured. Please send the log file to the developers.");

    wxDebugReport report;
    wxDebugReportPreviewStd preview;

    report.AddExceptionContext();
    report.AddExceptionDump();

    if ( preview.Show(report) )
        report.Process();
}

//-----------------------------------//

static EditorFrame* editorInstance;
EditorFrame& GetEditor() { return *editorInstance; }

//-----------------------------------//

EditorFrame::EditorFrame(const wxString& title)
	: wxFrame(nullptr, wxID_ANY, title)
	, engine(nullptr)
	, mainSplitter(nullptr)
	, viewSplitter(nullptr)
	, auiManager(nullptr)
	, toolBar(nullptr)
	, notebookCtrl(nullptr)
	, viewframe(nullptr)
	, undoManager(nullptr)
	, eventManager(nullptr)
	, pluginManager(nullptr)
	, pluginManagerFrame(nullptr)
{
	editorInstance = this;

	createUI();
	createEngine();
	createViews();
	createResources();
	createServices();
	createScene();
	createPlugins();
	createToolbar();
	createLastUI();

	ResourceManager* res = engine->getResourceManager();
	res->loadQueuedResources();

	// Update at least once before rendering.
	onUpdate(0);

	RenderControl* control = viewframe->getControl();
	control->startFrameLoop();
}

//-----------------------------------//

EditorFrame::~EditorFrame()
{
	getAUI()->UnInit();
	delete auiManager;

 	delete eventManager;
	delete pluginManager;
	delete undoManager;

	editorScene.reset();
	delete engine;
}

//-----------------------------------//

#define PLUGIN(name)								\
	plugin = new name##Plugin(this);				\
	pluginManager->registerPlugin( plugin );		\
	pluginManager->enablePlugin( plugin );			\

void EditorFrame::createPlugins()
{
	Plugin* plugin = nullptr;

	PLUGIN(Project);
	PLUGIN(Undo);
	PLUGIN(Scene);
	PLUGIN(Property);
	PLUGIN(Selection);
	PLUGIN(Gizmo);
	PLUGIN(Terrain);
	//PLUGIN(Camera);
	PLUGIN(Console);
	PLUGIN(Log);
	PLUGIN(Resources);
	PLUGIN(Sample);

	pluginManagerFrame = new PluginManagerFrame(this, pluginManager);

	wxBitmap icon = wxMEMORY_BITMAP(cog);
	
	wxAuiPaneInfo pane;
	pane.Caption("Plugins").Right().Dock().Icon(icon).Hide();

	getAUI()->AddPane(pluginManagerFrame, pane);

	ScenePtr scene = engine->getSceneManager();
	eventManager->onSceneLoad(scene);
}

//-----------------------------------//

void EditorFrame::createEngine()
{
	engine = new Engine;
	engine->create(VAPOR_EDITOR_NAME);
	engine->init(false);
}

//-----------------------------------//

void EditorFrame::createViews()
{	
	viewframe = new Viewframe( viewSplitter );
	viewSplitter->SetWindow( 0, viewframe );
	viewSplitter->SetExpanded( viewframe );

	RenderControl* control = viewframe->getControl();
	control->onRender.Connect( this, &EditorFrame::onRender );
	control->onUpdate.Connect( this, &EditorFrame::onUpdate );
	control->SetDropTarget( new ResourceDropTarget(this) );
	control->SetFocus();

	RenderDevice* renderDevice = engine->getRenderDevice();
	Window* window = (Window*) control->getRenderWindow(); 

	renderDevice->setWindow( window );
	renderDevice->setRenderTarget( window );

	engine->setupInput();
	inputManager = control->getInputManager();

	RenderView* view = viewframe->createView();
	view->setClearColor( Color(0.0f, 0.10f, 0.25f) );

	renderDevice->init();

#ifdef VAPOR_PHYSICS_BULLET
	PhysicsManager* physics = engine->getPhysicsManager();
	physics->createWorld();
#endif
}

//-----------------------------------//

void EditorFrame::createScene()
{
	// Create a scene node with editor stuff only.
	editorScene.reset( new Scene() );
	
	// Create a nice grid for the editor.
	EntityPtr nodeGrid( new Entity("Grid") );
	nodeGrid->addTransform();
	nodeGrid->addComponent( GridPtr( new Grid() ) );
	nodeGrid->setTag( Tags::NonPickable, true );
	editorScene->add( nodeGrid );

	Vector3 initialCameraPosition(0, 20, -65);

	EntityPtr nodeCamera( createCamera() );
	nodeCamera->getTransform()->setPosition(initialCameraPosition);
	editorScene->add( nodeCamera );

	CameraPtr camera = nodeCamera->getComponent<Camera>();
	viewframe->setMainCamera(camera);
	viewframe->switchToDefaultCamera();
}

//-----------------------------------//

void EditorFrame::createResources()
{
	// Mount the editor default media directories.
	FileSystem* fs = engine->getFileSystem();
	fs->mountDefaultLocations();
}

//-----------------------------------//

void EditorFrame::createServices()
{
	pluginManager = new PluginManager(this);
	eventManager = new Events(this);
	undoManager = new UndoManager();
}

//-----------------------------------//

EntityPtr EditorFrame::createCamera()
{
	// So each camera will have unique names.
	static byte i = 0;

	// Create a new first-person camera for our view.
	// By default it will be in perspective projection.
	CameraPtr camera( new Camera() );
	ComponentPtr cameraController( new FirstPersonController() );

	Frustum& frustum = camera->getFrustum();
	frustum.farPlane = 10000;

	// Generate a new unique name.
	std::string name( "EditorCamera"+String::fromNumber(i++) );

	EntityPtr entityCamera( new Entity(name) );
	entityCamera->addTransform();
	entityCamera->addComponent( camera );
	entityCamera->addComponent( cameraController );

#ifdef VAPOR_AUDIO_OPENAL
	ComponentPtr listener( new Listener() );
	entityCamera->addComponent( listener );
#endif

	return entityCamera;
}

//-----------------------------------//

void EditorFrame::createUI()
{
    SetIcon( wxIcon("editor") );

	auiManager = new wxAuiManager;
	auiManager->SetManagedWindow(this);

	createSplitter();
	createMenus();
	createNotebook();

	int style = wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_OVERFLOW;
	toolBar = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, style);

	toolBar->Bind(wxEVT_COMMAND_MENU_SELECTED, &EditorFrame::OnToolbarButtonClick, this);
}

//-----------------------------------//

void EditorFrame::createSplitter()
{
	viewSplitter = new wxFourWaySplitter(this);
	auiManager->AddPane(viewSplitter, wxAuiPaneInfo().CentrePane().Maximize());
}

//-----------------------------------//

void EditorFrame::createToolbar()
{
	if(!toolBar) return;

	toolBar->AddTool( Toolbar_ToogleGrid, "Grid", 
		wxMEMORY_BITMAP(grid_icon), "Show/hide the editor grid", wxITEM_CHECK );

	toolBar->AddTool( Toolbar_TooglePhysicsDebug, "Physics", 
		wxMEMORY_BITMAP(grid_icon), "Show/hide the physics debug", wxITEM_CHECK );

	toolBar->ToggleTool( Toolbar_ToogleGrid, true );

	toolBar->AddTool( Toolbar_TooglePlay, "Play", wxMEMORY_BITMAP(resultset_next), 
		"Enable/disable Play mode", wxITEM_CHECK );

	toolBar->AddSeparator();

	toolBar->AddTool( Toolbar_ToogleViewport, "Toogles maximize view", 
		wxMEMORY_BITMAP(application_split), "Toogles maximize view" );

	toolBar->AddTool( Toolbar_ToogleSidebar, "Shows/hides the sidebar", 
		wxMEMORY_BITMAP(application_side_tree_right), "Shows/hides the sidebar" );

	toolBar->AddTool( Toolbar_TooglePlugin, "Shows/hides the plugin manager", 
		wxMEMORY_BITMAP(cog), "Shows/hides the plugin manager" );

	getAUI()->AddPane(toolBar, wxAuiPaneInfo().ToolbarPane().Caption("Toolbar").Top());
}

//-----------------------------------//

void EditorFrame::createNotebook()
{
	//notebookCtrl = new wxAuiNotebook(this);

	//wxImageList* img = new wxImageList(16, 16);
	//notebookCtrl->( img );

	//auiManager->AddPane(notebookCtrl, wxAuiPaneInfo().Right().MinSize(220, -1));
}

//-----------------------------------//

void EditorFrame::redrawView()
{
	if( !viewframe )
		return;

	viewframe->flagRedraw();
}

//-----------------------------------//

void EditorFrame::onRender()
{
	RenderView* view = viewframe->getView();

	if( !view->getCamera() )
		viewframe->switchToDefaultCamera();

	const CameraPtr& camera = view->getCamera();
	if( !camera ) return;
	
	camera->setView( view );

	RenderBlock block;
	camera->cull( block, engine->getSceneManager() );
	camera->cull( block, editorScene );
	camera->render(block);

#ifdef VAPOR_PHYSICS_BULLET
	PhysicsManager* physics = engine->getPhysicsManager();
	physics->drawDebugWorld();
#endif
}

//-----------------------------------//

void EditorFrame::onUpdate( double delta )
{
	engine->update( delta );
	
	if(editorScene)
		editorScene->update( delta );
	//eventManager->onSceneUpdate();
}

//-----------------------------------//

CameraPtr EditorFrame::getPlayerCamera() const
{
	ScenePtr scene = engine->getSceneManager();
	CameraPtr camera;
	
	const std::vector<EntityPtr> entities = scene->getEntities();
	
	for( uint i = 0; i < entities.size(); i++ )
	{
		const EntityPtr& node = entities[i];
		camera = node->getComponent<Camera>();

		if( camera )
			break;
	}

	return camera;
}
		
//-----------------------------------//

void EditorFrame::switchPlayMode(bool switchToPlay)
{
#ifdef VAPOR_PHYSICS_BULLET
	// Toogle the physics simulation state.
	PhysicsManager* physics = engine->getPhysicsManager();
	
	if( physics )
		physics->setSimulation( switchToPlay );
#endif

	CameraPtr camera = getPlayerCamera();
	EntityPtr nodeCamera;
	ControllerPtr controller;

	if( camera )
		nodeCamera = camera->getEntity()->getShared();

	if( !nodeCamera )
		return;

	if( controller )
		controller = nodeCamera->getComponentFromFamily<Controller>();

	if( switchToPlay )
	{
		// Change the active camera.
		RenderView* view = viewframe->getView();

		if( controller )
			controller->setEnabled(true);

		if( camera )
			view->setCamera(camera);
	}
	else
	{
		if( controller )
			controller->setEnabled(false);

		viewframe->switchToDefaultCamera();
	}
}

//-----------------------------------//

void EditorFrame::OnToolbarButtonClick(wxCommandEvent& event)
{
	const int id = event.GetId();

	switch(id) 
	{
	//-----------------------------------//
	case Toolbar_TooglePlugin:
	{
		wxAuiPaneInfo& pane = getAUI()->GetPane(pluginManagerFrame);
		pane.Show( !pane.IsShown() );
		getAUI()->Update();

		break;
	}
	//-----------------------------------//
	case Toolbar_ToogleGrid:
	{
		const EntityPtr& grid = editorScene->findEntity("Grid");
		
		if( grid )
			grid->setVisible( !grid->isVisible() );
		
		redrawView();
		
		break;
	}
	//-----------------------------------//
	case Toolbar_TooglePhysicsDebug:
	{
#ifdef VAPOR_PHYSICS_BULLET
		Engine* engine = GetEngine();
		PhysicsManager* physics = engine->getPhysicsManager();
		
		if( physics )
			physics->setDebugWorld( !physics->getDebugWorld() );
		
		redrawView();
#endif
		break;
	}
	//-----------------------------------//
	case Toolbar_TooglePlay:
	{
		bool switchToPlay = event.IsChecked();
		switchPlayMode(switchToPlay);

		break;
	}
	//-----------------------------------//
	case Toolbar_ToogleViewport:
	{
		int curExpansion = viewSplitter->GetExpanded();

		if( curExpansion >= 0 )
			viewSplitter->SetExpanded(-1);
		else
			viewSplitter->SetExpanded(0);

		break;
	}
	//-----------------------------------//
	case Toolbar_ToogleSidebar:
	{
		//if( !mainSplitter->GetWindow2() )
		//	mainSplitter->SplitVertically(viewSplitter, notebookCtrl, -220);
		//else
		//	mainSplitter->Unsplit();

		//break;
	}
	//-----------------------------------//
	} // end switch
}

//-----------------------------------//

} } // end namespaces