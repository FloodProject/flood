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
#include "ResourceDrop.h"
#include "wxFourWaySplitter.h"
#include "StackWalker.h"

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
	StackWalkerLog sw;
	sw.ShowCallstack();
	sw.Log();

	wxMessageOutputBest output;
	output.Output("An exception occured. Please send the log file to the developers.");
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

	getAUI()->Update();

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
	PLUGIN(Console);
	PLUGIN(Selection);
	PLUGIN(Gizmo);
	PLUGIN(Resources);
	//PLUGIN(Terrain);
	PLUGIN(Camera);
	PLUGIN(Log);
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

	RenderDevice* device = engine->getRenderDevice();
	Window* window = (Window*) control->getRenderWindow(); 

	device->setWindow( window );
	device->setRenderTarget( window );

	engine->setupInput();
	inputManager = control->getInputManager();

	RenderView* view = viewframe->createView();
	view->setClearColor( Color(0.0f, 0.10f, 0.25f) );

	device->init();

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

	EntityPtr nodeCamera( createCamera() );
	nodeCamera->getTransform()->translate( 0, 20, -65 );
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

	ResourceManager* res = engine->getResourceManager();
	std::vector<std::string> files = System::enumerateFiles("Media/Shaders");

	for(uint i = 0; i < files.size(); i++ )
		res->loadResource(files[i]);
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

	Bind(wxEVT_COMMAND_MENU_SELECTED, &EditorFrame::OnQuit, this, Editor_Quit);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &EditorFrame::OnAbout, this, Editor_About);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &EditorFrame::OnToolbarButtonClick, this);
	Bind(wxEVT_KEY_DOWN, &EditorFrame::OnKeyDown, this);
	Bind(wxEVT_KEY_UP, &EditorFrame::OnKeyUp, this);
	
	Bind(wxEVT_LEFT_DOWN, &EditorFrame::OnMouseEvent, this);
	Bind(wxEVT_LEFT_UP, &EditorFrame::OnMouseEvent, this);
	Bind(wxEVT_LEFT_DCLICK, &EditorFrame::OnMouseEvent, this);
	Bind(wxEVT_MIDDLE_DOWN, &EditorFrame::OnMouseEvent, this);
	Bind(wxEVT_MIDDLE_UP, &EditorFrame::OnMouseEvent, this);
	Bind(wxEVT_MIDDLE_DCLICK, &EditorFrame::OnMouseEvent, this);
	Bind(wxEVT_RIGHT_DOWN, &EditorFrame::OnMouseEvent, this);
	Bind(wxEVT_RIGHT_UP, &EditorFrame::OnMouseEvent, this);
	Bind(wxEVT_RIGHT_DCLICK, &EditorFrame::OnMouseEvent, this);
	Bind(wxEVT_AUX1_DOWN, &EditorFrame::OnMouseEvent, this);
	Bind(wxEVT_AUX1_UP, &EditorFrame::OnMouseEvent, this);
	Bind(wxEVT_AUX1_DCLICK, &EditorFrame::OnMouseEvent, this);
	Bind(wxEVT_AUX2_DOWN, &EditorFrame::OnMouseEvent, this);
	Bind(wxEVT_AUX2_UP, &EditorFrame::OnMouseEvent, this);
	Bind(wxEVT_MOTION, &EditorFrame::OnMouseEvent, this);
	Bind(wxEVT_LEAVE_WINDOW, &EditorFrame::OnMouseEvent, this);
	Bind(wxEVT_ENTER_WINDOW, &EditorFrame::OnMouseEvent, this);
	Bind(wxEVT_MOUSEWHEEL, &EditorFrame::OnMouseEvent, this);
}

//-----------------------------------//

void EditorFrame::createSplitter()
{
	viewSplitter = new wxFourWaySplitter(this);
	auiManager->AddPane(viewSplitter, wxAuiPaneInfo().CentrePane().Maximize());
}

//-----------------------------------//

void EditorFrame::createMenus()
{
    // create a menu bar
    wxMenu* fileMenu = new wxMenu;
	fileMenu->Append(Editor_Quit, "E&xit\tAlt-X", "Quit this program");

    wxMenu* toolsMenu = new wxMenu;

    // the "About" item should be in the help menu
    wxMenu* helpMenu = new wxMenu;
    helpMenu->Append(Editor_About, "&About...\tF1", "Show about dialog");

    // now append the freshly created menu to the menu bar...
    wxMenuBar* menuBar = new wxMenuBar();
    menuBar->Append(fileMenu, "&File");
	menuBar->Append(toolsMenu, "&Tools");
    menuBar->Append(helpMenu, "&Help");

    // ... and attach this menu bar to the frame
    SetMenuBar(menuBar);
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
		nodeCamera = camera->getEntity();

	if( !nodeCamera )
		return;

	if( controller )
		controller = nodeCamera->getTypedComponent<Controller>();

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

void EditorFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    // true forces the frame to close.
    Close(true);
}

//-----------------------------------//

void EditorFrame::OnKeyDown(wxKeyEvent& event)
{
	inputManager->processKeyEvent(event, true);
}

//-----------------------------------//

void EditorFrame::OnKeyUp(wxKeyEvent& event)
{
	inputManager->processKeyEvent(event, false);
}

//-----------------------------------//

void EditorFrame::OnMouseEvent(wxMouseEvent& event)
{
	inputManager->processMouseEvent(event);
}

//-----------------------------------//

void EditorFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
	wxFrame* about = new wxFrame(this, wxID_ANY, "About " VAPOR_EDITOR_NAME,
		wxDefaultPosition, wxDefaultSize, wxCAPTION|wxCLOSE_BOX|wxSYSTEM_MENU|
		wxFRAME_FLOAT_ON_PARENT|wxFRAME_TOOL_WINDOW|wxTAB_TRAVERSAL);

	wxBoxSizer* bSizer1 = new wxBoxSizer( wxVERTICAL );
	wxBoxSizer* bSizer2 = new wxBoxSizer( wxVERTICAL );

	wxPanel* m_panel1 = new wxPanel( about );

	wxBitmap aboutIcon = wxMEMORY_BITMAP(::about);
	wxStaticBitmap* m_bitmap1 = new wxStaticBitmap( m_panel1, wxID_ANY, aboutIcon);
	bSizer2->Add( m_bitmap1, 0, wxEXPAND|wxALIGN_CENTER_HORIZONTAL, 0 );

	wxString aboutText(
		"This software is © 2009-2010 João Matos and the rest of the team.\n\n"
		VAPOR_EDITOR_NAME " uses some free software packages: wxWidgets (wxWidgets.org),"
		" Lua (lua.org),\nBullet (bulletphysics.com), Boost (boost.org), zlib (zlib.org)"
		" and the list goes on.\n\nCheck the documentation provided with the software"
		" for more details.");

	wxStaticText* m_staticText2 = new wxStaticText( m_panel1, wxID_ANY, aboutText );
	m_staticText2->Wrap( -1 );
	bSizer2->Add( m_staticText2, 0, wxEXPAND|wxTOP|wxBOTTOM, 5 );
	
	wxBoxSizer* bSizer3 = new wxBoxSizer( wxHORIZONTAL );
	
	wxStaticLine* m_staticline1 = new wxStaticLine( m_panel1, wxID_ANY );
	bSizer3->Add( m_staticline1, 1, wxALL|wxALIGN_CENTER_VERTICAL, 10 );
	
	wxHyperlinkCtrl* m_hyperlink1 = new wxHyperlinkCtrl( m_panel1, wxID_ANY,
		"vapor3D (http://www.vapor3d.org)", "http://www.vapor3d.org" );
	bSizer3->Add( m_hyperlink1, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	//wxStaticText* m_hyperlink1 = new wxStaticText( m_panel1, wxID_ANY,
	//	"http://www.vapor3d.org" );
	//bSizer3->Add( m_hyperlink1, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxStaticLine* m_staticline2 = new wxStaticLine( m_panel1, wxID_ANY );
	bSizer3->Add( m_staticline2, 1, wxALL|wxALIGN_CENTER_VERTICAL, 10 );
	
	bSizer2->Add( bSizer3, 1, wxEXPAND, 5 );
	
	m_panel1->SetSizer( bSizer2 );
	m_panel1->Layout();

	bSizer2->Fit( m_panel1 );
	bSizer1->Add( m_panel1, 1, wxEXPAND, 5 );
	
	about->SetSizer( bSizer1 );
	about->Layout();
	bSizer1->Fit( about );

	about->Show(true);
}

//-----------------------------------//

} } // end namespaces