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

// Editor plugins
#include "tools/Project/ProjectPlugin.h"
#include "tools/UndoRedo/UndoPlugin.h"
#include "tools/Gizmos/GizmoPlugin.h"
#include "tools/Terrain/TerrainPlugin.h"
#include "tools/Camera/CameraPlugin.h"
#include "tools/Sample/SamplePlugin.h"

namespace vapor { namespace editor {

// ----------------------------------------------------------------------
// event tables and other macros for wxWidgets
// ----------------------------------------------------------------------

BEGIN_EVENT_TABLE(EditorFrame, wxFrame)
    EVT_MENU(Editor_Quit, EditorFrame::OnQuit)
    EVT_MENU(Editor_About, EditorFrame::OnAbout)
	EVT_MENU(wxID_ANY, EditorFrame::OnToolbarButtonClick)
	EVT_KEY_DOWN(EditorFrame::OnKeyDown)
	EVT_KEY_UP(EditorFrame::OnKeyUp)
	EVT_MOUSE_EVENTS(EditorFrame::OnMouseEvent)
END_EVENT_TABLE()

IMPLEMENT_APP(EditorApp)

//-----------------------------------//

bool EditorApp::OnInit()
{
    // Call the base class initialization method, currently it only parses a
    // few common command-line options but it could be do more in the future.
    if( !wxApp::OnInit() )
	{
        return false;
	}

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
	, viewframe(nullptr)
	, pluginManagerFrame(nullptr)
	, undoManager(nullptr)
{
    // Set the editor icon.
    SetIcon( wxIcon("editor") );
	
	// Initialize the engine.
	initEngine();

	undoManager = new UndoManager();

	createMenus();
	createToolbar();
	createStatusbar();
	createNotebook();
	createPlugins();

	createScene();
	createEditorScene();

	Viewport* viewport = viewframe->getViewport();
	RenderTarget* renderTarget = viewport->getRenderTarget();
	renderTarget->makeCurrent();

	toolBar->Realize();
	SetSizerAndFit( sizer );

	waitFinishLoad();

	//SetFocusIgnoringChildren();
}

//-----------------------------------//

EditorFrame::~EditorFrame()
{
	delete pluginManager;
	delete undoManager;

	// Stop the frame events.
	viewframe->Destroy();
	editorScene.reset();
	delete engine;
}

//-----------------------------------//

void EditorFrame::createPlugins()
{
	pluginManager = new PluginManager(this);
	
	Plugin* plugin = nullptr;

	plugin = new ProjectPlugin(this);
	pluginManager->registerPlugin( plugin );
	pluginManager->enablePlugin( plugin );

	plugin = new UndoPlugin(this);
	pluginManager->registerPlugin( plugin );
	pluginManager->enablePlugin( plugin );

	plugin = new GizmoPlugin(this);
	pluginManager->registerPlugin( plugin );
	pluginManager->enablePlugin( plugin );

	plugin = new TerrainPlugin(this);
	pluginManager->registerPlugin( plugin );

	plugin = new CameraPlugin(this);
	pluginManager->registerPlugin( plugin );
	pluginManager->enablePlugin( plugin );

	plugin = new SamplePlugin(this);
	pluginManager->registerPlugin( plugin );

	pluginManagerFrame = new PluginManagerFrame( this, pluginManager);
}

//-----------------------------------//

void EditorFrame::initEngine()
{
	engine = Engine::getInstancePtr();
	engine->create( VAPOR_EDITOR_NAME, nullptr, false );
	engine->init( false );

	// Create a scene node with editor stuff only.
	editorScene.reset( new Scene() );

	createMainViewframe();

	RenderDevice* device = engine->getRenderDevice();
	device->init();

	// Mount the editor default media VFS directories.
	FileSystem* fs = engine->getFileSystem();
	fs->mountDefaultLocations();
}

//-----------------------------------//

void EditorFrame::waitFinishLoad()
{
	ResourceManager* const rm = engine->getResourceManager();
	rm->waitUntilQueuedResourcesLoad();

	// Update at least once before rendering.
	onUpdate( 0.0f );

	RenderControl* control = viewframe->getControl();
	control->startFrameLoop();
}

//-----------------------------------//

void EditorFrame::createMainViewframe()
{
	viewframe = new Viewframe( this );

	RenderControl* control = viewframe->getControl();
	control->onRender += fd::bind( &EditorFrame::onRender, this );
	control->onUpdate += fd::bind( &EditorFrame::onUpdate, this );
	control->SetFocus();

	RenderDevice* device = engine->getRenderDevice();
	Window* window = (Window*) control->getRenderWindow(); 

	device->setWindow( window );
	device->setRenderTarget( window );

	engine->setupInput();
	im = control->getInputManager();

	NodePtr camera( createCamera() );	
	editorScene->add( camera );

	Viewport* viewport = viewframe->createViewport( camera );
	viewport->setClearColor( Color(0.0f, 0.10f, 0.25f) );

	TransformPtr transform( camera->getTransform() );
	transform->translate( 0.0f, 20.0f, -65.0f );

	sizer = new wxBoxSizer( wxHORIZONTAL );
	sizer->Add( viewframe, 1, wxEXPAND|wxALL );
}

//-----------------------------------//

NodePtr EditorFrame::createCamera()
{
	// So each camera will have unique names.
	static byte i = 0;

	RenderDevice* device = engine->getRenderDevice();
	
	// Create a new first-person camera for our viewport.
	// By default it will be in perspective projection.
	CameraPtr camera( new FirstPersonCamera(device) );

	// Generate a new unique name.
	std::string name( "EditorCamera"+String::fromNumber(i++) );

	NodePtr cameraNode( new Node(name) );
	cameraNode->addTransform();
	cameraNode->addComponent( camera );
	cameraNode->setTag( Tags::NonPickable, true );
	cameraNode->setTag( EditorTags::EditorOnly, true );

	return cameraNode;
}

//-----------------------------------//

void EditorFrame::createNotebook()
{
	wxImageList* img = new wxImageList(16, 16, false, 5);
	img->Add(wxMEMORY_BITMAP(sitemap_color));
	img->Add(wxMEMORY_BITMAP(package));
	img->Add(wxMEMORY_BITMAP(world));

	notebookCtrl = new wxNotebook( this, wxID_ANY );
	notebookCtrl->AssignImageList( img );

	//-----------------------------------//

	wxPanel* panelScene = new wxPanel( notebookCtrl, wxID_ANY );
	
	wxBoxSizer* panelSceneSizer = new wxBoxSizer( wxVERTICAL );
	
	sceneTreeCtrl = new SceneTreeCtrl( this, engine, panelScene, wxID_ANY );

	sceneTreeCtrl->onItemSelected += fd::bind( &EditorFrame::OnNodeSelected, this );

	panelSceneSizer->Add( sceneTreeCtrl, 1, wxALL|wxEXPAND, 0 );

	panelScene->SetSizerAndFit( panelSceneSizer );

	int scenePage = notebookCtrl->AddPage( panelScene, wxT("Scene"), true, 0 );

	sizer->Add(notebookCtrl, 0, wxALL|wxEXPAND, 0 );

	//-----------------------------------//

	wxPanel* panelResources = new wxPanel( notebookCtrl, wxID_ANY );
	
	wxBoxSizer* panelResourcesSizer = new wxBoxSizer( wxVERTICAL );
	
	resourceTreeCtrl = new ResourceTreeCtrl(panelResources, wxID_ANY,
		wxDefaultPosition, wxSize(220, -1), this);

	panelResourcesSizer->Add( resourceTreeCtrl, 1, wxALL|wxEXPAND, 0 );

	panelResources->SetSizerAndFit( panelResourcesSizer );

	notebookCtrl->AddPage( panelResources, wxT("Resources"), true, 1 );

	notebookCtrl->SetSelection( scenePage );

	//-----------------------------------//

	notebookCtrl->ChangeSelection( scenePage );
}

//-----------------------------------//

void EditorFrame::createStatusbar()
{
    // create a status bar just for fun (by default with 1 pane only)
    //CreateStatusBar(2);
	//SetStatusText("vaporEngine (FPS: OVER 9000!)");
}

//-----------------------------------//

void EditorFrame::createMenus()
{
    // create a menu bar
    wxMenu* fileMenu = new wxMenu;
	fileMenu->Append(Editor_Quit, "E&xit\tAlt-X", "Quit this program");

    // the "About" item should be in the help menu
    wxMenu* helpMenu = new wxMenu;
    helpMenu->Append(Editor_About, "&About...\tF1", "Show about dialog");

    // now append the freshly created menu to the menu bar...
    wxMenuBar* menuBar = new wxMenuBar();
    menuBar->Append(fileMenu, "&File");
    menuBar->Append(helpMenu, "&Help");

    // ... and attach this menu bar to the frame
    SetMenuBar(menuBar);
}

//-----------------------------------//

void EditorFrame::createToolbar()
{
	toolBar = CreateToolBar( wxTB_HORIZONTAL, wxID_ANY );

	toolBar->AddTool( Toolbar_ToogleConsole, "Console", 
		wxMEMORY_BITMAP(application_xp_terminal), "Open/close the scripting console" ); 

	toolBar->AddTool( Toolbar_ToogleGrid, "Grid", 
		wxMEMORY_BITMAP(grid_icon), "Show/hide the editor grid", wxITEM_CHECK );

	toolBar->ToggleTool( Toolbar_ToogleGrid, true );

	toolBar->AddTool( Toolbar_TooglePlay, "Play", wxMEMORY_BITMAP(resultset_next), 
		"Enable/disable Play mode", wxITEM_CHECK );

	// --------------
	// UI tools
	// --------------

	toolBar->AddSeparator();

	toolBar->AddTool( Toolbar_ToogleSidebar, "Shows/hides the sidebar", 
		wxMEMORY_BITMAP(application_side_tree_right), "Shows/hides the sidebar" );

	toolBar->AddTool( Toolbar_TooglePlugin, "Shows/hides the plugin manager", 
		wxMEMORY_BITMAP(page_white_text), "Shows/hides the plugin manager" );
}

//-----------------------------------//

void EditorFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
	wxFrame* about = new wxFrame(this, wxID_ANY, "About " VAPOR_EDITOR_NAME, wxDefaultPosition,
		wxDefaultSize, wxCAPTION|wxCLOSE_BOX|wxFRAME_FLOAT_ON_PARENT|wxFRAME_TOOL_WINDOW|
		wxSYSTEM_MENU|wxTAB_TRAVERSAL );

	wxBoxSizer* bSizer1 = new wxBoxSizer( wxVERTICAL );
	wxBoxSizer* bSizer2 = new wxBoxSizer( wxVERTICAL );

	wxPanel* m_panel1 = new wxPanel( about );

	wxStaticBitmap* m_bitmap1 = new wxStaticBitmap( m_panel1, wxID_ANY, wxMEMORY_BITMAP(::about) );
	bSizer2->Add( m_bitmap1, 0, wxEXPAND|wxALIGN_CENTER_HORIZONTAL, 0 );

	wxString aboutText( "This software is © 2009-2010 João Matos and the rest of the team.\n\n"
		VAPOR_EDITOR_NAME " uses some free software packages: wxWidgets (wxWidgets.org), Lua (lua.org),\n"
		"Bullet (bulletphysics.com), Boost (boost.org), zlib (zlib.org) and the list goes on.\n\n"
		"Check the documentation provided with the software for more details." );

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