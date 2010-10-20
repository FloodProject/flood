/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "Editor.h"
#include "EditorIcons.h"
#include "RenderControl.h"
#include "PluginManager.h"
#include "PluginManagerFrame.h"
#include "UndoManager.h"
#include "Viewframe.h"
#include "EditorInputManager.h"
#include "Events.h"
#include "wxFourWaySplitter.h"

// Editor plugins
#include "plugins/Project/ProjectPlugin.h"
#include "plugins/UndoRedo/UndoPlugin.h"
#include "plugins/Scene/ScenePlugin.h"
#include "plugins/Console/ConsolePlugin.h"
#include "plugins/Resources/ResourcesPlugin.h"
#include "plugins/Property/PropertyPlugin.h"
#include "plugins/Gizmos/GizmoPlugin.h"
#include "plugins/Terrain/TerrainPlugin.h"
#include "plugins/Camera/CameraPlugin.h"
#include "plugins/Sample/SamplePlugin.h"

namespace vapor { namespace editor {

//-----------------------------------//

IMPLEMENT_APP(EditorApp)

bool EditorApp::OnInit()
{
    if( !wxApp::OnInit() )
        return false;

	//wxHandleFatalExceptions(true);

	// Register a PNG image handler (so toolbars icons can be decoded).
	wxImage::AddHandler(new wxPNGHandler);

    EditorFrame* frame = new EditorFrame( VAPOR_EDITOR_NAME );
	frame->SetSize(750, 500);
    frame->Show(true);

	SetTopWindow( frame );

    // wxApp::OnRun() will be called which will enter the main message
    // loop and the application will run. If we returned false here, the
    // application would exit immediately.
    return true;
}

//-----------------------------------//

EditorFrame::EditorFrame(const wxString& title)
	: wxFrame(nullptr, wxID_ANY, title)
	, engine(nullptr)
	, mainSplitter(nullptr)
	, viewSplitter(nullptr)
	, viewframe(nullptr)
	, undoManager(nullptr)
	, eventManager(nullptr)
	, pluginManagerFrame(nullptr)
{
	createUI();
	createEngine();
	createViews();
	createResources();
	createScene();
	createServices();
	createPlugins();
	toolBar->Realize();
	SetSizerAndFit( sizer );

	ResourceManager* const rm = engine->getResourceManager();
	rm->waitUntilQueuedResourcesLoad();

	// Update at least once before rendering.
	onUpdate( 0 );

	RenderControl* control = viewframe->getControl();
	control->startFrameLoop();
}

//-----------------------------------//

EditorFrame::~EditorFrame()
{
 	delete eventManager;
	delete pluginManager;
	delete undoManager;
	mainSplitter->Destroy();

	editorScene.reset();
	delete engine;
}

//-----------------------------------//

void EditorFrame::createPlugins()
{
	Plugin* plugin = nullptr;

	plugin = new ProjectPlugin(this);
	pluginManager->registerPlugin( plugin );
	pluginManager->enablePlugin( plugin );

	plugin = new UndoPlugin(this);
	pluginManager->registerPlugin( plugin );
	pluginManager->enablePlugin( plugin );

	plugin = new ScenePlugin(this);
	pluginManager->registerPlugin( plugin );
	pluginManager->enablePlugin( plugin );

	plugin = new PropertyPlugin(this);
	pluginManager->registerPlugin( plugin );
	pluginManager->enablePlugin( plugin );

	plugin = new ConsolePlugin(this);
	pluginManager->registerPlugin( plugin );
	pluginManager->enablePlugin( plugin );

	plugin = new ResourcesPlugin(this);
	pluginManager->registerPlugin( plugin );
	pluginManager->enablePlugin( plugin );

	plugin = new GizmoPlugin(this);
	pluginManager->registerPlugin( plugin );
	pluginManager->enablePlugin( plugin );

	plugin = new TerrainPlugin(this);
	pluginManager->registerPlugin( plugin );
	pluginManager->enablePlugin( plugin );

	plugin = new CameraPlugin(this);
	pluginManager->registerPlugin( plugin );
	pluginManager->enablePlugin( plugin );

	plugin = new SamplePlugin(this);
	pluginManager->registerPlugin( plugin );

	pluginManagerFrame = new PluginManagerFrame( this, pluginManager);
	pluginManagerFrame->SetInitialSize(wxSize(200, 350));
}

//-----------------------------------//

void EditorFrame::createEngine()
{
	engine = Engine::getInstancePtr();
	engine->create( VAPOR_EDITOR_NAME, nullptr, false );
	engine->init( false );

	// Mount the editor default media VFS directories.
	FileSystem* fs = engine->getFileSystem();
	fs->mountDefaultLocations();
}

//-----------------------------------//

void EditorFrame::createViews()
{	
	viewframe = new Viewframe( viewSplitter );
	viewSplitter->SetWindow( 0, viewframe );
	viewSplitter->SetExpanded( viewframe );

	RenderControl* control = viewframe->getControl();
	control->onRender += fd::bind( &EditorFrame::onRender, this );
	control->onUpdate += fd::bind( &EditorFrame::onUpdate, this );
	control->SetDropTarget(this);
	control->SetFocus();

	RenderDevice* device = engine->getRenderDevice();
	Window* window = (Window*) control->getRenderWindow(); 

	device->setWindow( window );
	device->setRenderTarget( window );

	engine->setupInput();
	inputManager = control->getInputManager();

	View* view = viewframe->createView();
	view->setClearColor( Color(0.0f, 0.10f, 0.25f) );

	engine->getRenderDevice()->init();
	engine->getPhysicsManager()->createWorld();
}

//-----------------------------------//

void EditorFrame::createScene()
{
	// Create a scene node with editor stuff only.
	editorScene.reset( new Scene() );
	
	// Create a nice grid for the editor.
	NodePtr nodeGrid( new Node("Grid") );
	nodeGrid->addTransform();
	nodeGrid->addComponent( GridPtr( new Grid() ) );
	nodeGrid->setTag( Tags::NonPickable, true );
	editorScene->add( nodeGrid );

	NodePtr nodeCamera( createCamera() );
	nodeCamera->getTransform()->translate( 0, 20, -65 );
	editorScene->add( nodeCamera );

	CameraPtr camera = nodeCamera->getComponent<Camera>();
	viewframe->setMainCamera(camera);
	viewframe->switchToDefaultCamera();
}

//-----------------------------------//

void EditorFrame::createResources()
{
	ResourceManager* const rm = engine->getResourceManager();

	rm->loadResource("Diffuse.glsl");
	rm->loadResource("Tex.glsl");
	rm->loadResource("Toon.glsl");
	rm->loadResource("Tex_Toon.glsl");
	rm->loadResource("Tex_Toon_Skin.glsl");
	rm->loadResource("Sky.glsl");
	rm->loadResource("Water.glsl");
	rm->loadResource("ProjTex.glsl");
}

//-----------------------------------//

void EditorFrame::createServices()
{
	pluginManager = new PluginManager(this);
	eventManager = new Events(this);
	undoManager = new UndoManager();
}

//-----------------------------------//

NodePtr EditorFrame::createCamera()
{
	// So each camera will have unique names.
	static byte i = 0;

	RenderDevice* device = engine->getRenderDevice();
	
	// Create a new first-person camera for our view.
	// By default it will be in perspective projection.
	CameraPtr camera( new Camera(device) );
	ComponentPtr cameraController( new FirstPersonController() );

	Frustum& frustum = camera->getFrustum();
	frustum.farPlane = 10000;

	// Generate a new unique name.
	std::string name( "EditorCamera"+String::fromNumber(i++) );

	NodePtr nodeCamera( new Node(name) );
	nodeCamera->addTransform();
	nodeCamera->addComponent( camera );
	nodeCamera->addComponent( cameraController );
	nodeCamera->setTag( Tags::NonPickable, true );
	nodeCamera->setTag( EditorTags::EditorOnly, true );

	return nodeCamera;
}

//-----------------------------------//

void EditorFrame::createUI()
{
    SetIcon( wxIcon("editor") );

	// Creates the main sizer.
	sizer = new wxBoxSizer( wxHORIZONTAL );

	createSplitter();
	createMenus();
	createToolbar();
	createNotebook();

	mainSplitter->SplitVertically(viewSplitter, notebookCtrl, -220);

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

	SetDataObject(&data);
}

//-----------------------------------//

void EditorFrame::createSplitter()
{
	mainSplitter = new wxSplitterWindow(this);
	mainSplitter->SetSashGravity( 1.0 );
	//mainSplitter->SetMinimumPaneSize( );
	sizer->Add( mainSplitter, wxSizerFlags(1).Expand() );

	viewSplitter = new wxFourWaySplitter(mainSplitter);
	//sizer->Add( viewSplitter, wxSizerFlags(1).Expand() );
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
	toolBar = CreateToolBar( wxTB_HORIZONTAL, wxID_ANY );

	toolBar->AddTool( Toolbar_ToogleGrid, "Grid", 
		wxMEMORY_BITMAP(grid_icon), "Show/hide the editor grid", wxITEM_CHECK );

	toolBar->AddTool( Toolbar_TooglePhysicsDebug, "Physics", 
		wxMEMORY_BITMAP(grid_icon), "Show/hide the physics debug", wxITEM_CHECK );

	toolBar->ToggleTool( Toolbar_ToogleGrid, true );

	toolBar->AddTool( Toolbar_TooglePlay, "Play", wxMEMORY_BITMAP(resultset_next), 
		"Enable/disable Play mode", wxITEM_CHECK );

	toolBar->AddSeparator();

	toolBar->AddTool( Toolbar_ToogleViewport, "Toogles maximize view", 
		wxMEMORY_BITMAP(scale), "Toogles maximize view" );

	toolBar->AddTool( Toolbar_ToogleSidebar, "Shows/hides the sidebar", 
		wxMEMORY_BITMAP(application_side_tree_right), "Shows/hides the sidebar" );

	toolBar->AddTool( Toolbar_TooglePlugin, "Shows/hides the plugin manager", 
		wxMEMORY_BITMAP(page_white_text), "Shows/hides the plugin manager" );
}

//-----------------------------------//

void EditorFrame::createNotebook()
{
	notebookCtrl = new wxNotebook(mainSplitter, wxID_ANY);

	wxImageList* img = new wxImageList(16, 16);
	notebookCtrl->AssignImageList( img );
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
	View* view = viewframe->getView();

	if( !view->getCamera() )
		viewframe->switchToDefaultCamera();

	const CameraPtr& camera = view->getCamera();

	#pragma TODO("Renderables need to be sent in a single queue")

	camera->setView( view );
	camera->render( engine->getSceneManager() );
	camera->render( editorScene, false );

	PhysicsManager* physics = engine->getPhysicsManager();
	physics->drawDebugWorld();
}

//-----------------------------------//

void EditorFrame::onUpdate( double delta )
{
	engine->update( delta );
	editorScene->update( delta );
	//eventManager->onSceneUpdate();
}

//-----------------------------------//

CameraPtr EditorFrame::getPlayerCamera() const
{
	ScenePtr scene = engine->getSceneManager();
	CameraPtr camera;

	foreach( const NodePtr& node, scene->getChildren() )
	{
		camera = node->getComponent<Camera>();

		if( camera )
			break;
	}

	return camera;
}
		
//-----------------------------------//

void EditorFrame::switchPlayMode(bool switchToPlay)
{
	// Toogle the physics simulation state.
	PhysicsManager* physics = engine->getPhysicsManager();
	
	if( physics )
		physics->setSimulation( switchToPlay );

	CameraPtr camera = getPlayerCamera();
	NodePtr nodeCamera;
	ControllerPtr controller;

	if( camera )
		nodeCamera = camera->getNode();

	if( !nodeCamera )
		return;

	if( controller )
		controller = nodeCamera->getTypedComponent<Controller>();

	if( switchToPlay )
	{
		// Change the active camera.
		View* view = viewframe->getView();

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
		pluginManagerFrame->Show( !pluginManagerFrame->IsShown() );
		pluginManagerFrame->SetFocus();
		
		break;
	}
	//-----------------------------------//
	case Toolbar_ToogleGrid:
	{
		const NodePtr& grid = editorScene->getEntity( "Grid" );
		
		if( grid )
			grid->setVisible( !grid->isVisible() );
		
		redrawView();
		
		break;
	}
	//-----------------------------------//
	case Toolbar_TooglePhysicsDebug:
	{
		Engine* engine = Engine::getInstancePtr();
		PhysicsManager* physics = engine->getPhysicsManager();
		
		if( physics )
			physics->setDebugWorld( !physics->getDebugWorld() );
		
		redrawView();
		
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
		if( !mainSplitter->GetWindow2() )
			mainSplitter->SplitVertically(viewSplitter, notebookCtrl, -220);
		else
			mainSplitter->Unsplit();

		break;
	}
	//-----------------------------------//
	} // end switch
}

//-----------------------------------//

wxDragResult EditorFrame::OnData(wxCoord x, wxCoord y, wxDragResult def)
{
	dropCoords.x = x;
	dropCoords.y = y;

	return def;
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