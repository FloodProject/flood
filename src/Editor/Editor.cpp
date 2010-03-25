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

// Editor modes
#include "GizmoMode.h"
#include "TerrainMode.h"

namespace vapor { namespace editor {

// ----------------------------------------------------------------------
// event tables and other macros for wxWidgets
// ----------------------------------------------------------------------

BEGIN_EVENT_TABLE(EditorFrame, wxFrame)
    EVT_MENU(Editor_Quit,  EditorFrame::OnQuit)
    EVT_MENU(Editor_About, EditorFrame::OnAbout)
	EVT_MENU(wxID_ANY, EditorFrame::OnToolbarButtonClick)
	EVT_KEY_DOWN(EditorFrame::OnKeyDown)
END_EVENT_TABLE()

IMPLEMENT_APP(EditorApp)

//-----------------------------------//

bool EditorApp::OnInit()
{
    // Call the base class initialization method, currently it only parses a
    // few common command-line options but it could be do more in the future.
    if ( !wxApp::OnInit() )
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

// frame constructor
EditorFrame::EditorFrame(const wxString& title)
       : wxFrame(nullptr, wxID_ANY, title), engine( nullptr ), 
	   currentMode( nullptr )
{
    // Set the editor icon.
    SetIcon( wxIcon( "editor" ) );
	
	// Initialize the engine.
	initEngine();

	sizer = new wxBoxSizer( wxHORIZONTAL );
	sizer->Add( viewport, 1, wxEXPAND|wxALL );

	createModes();
	createMenus();
	createToolbar();
	createStatusbar();
	createNotebook();
	createScene();
	createEditorScene();

	toolBar->Realize();
	SetSizerAndFit( sizer );
	
	viewport->getControl()->SetFocus();
}

//-----------------------------------//

EditorFrame::~EditorFrame()
{
	foreach( const Mode* mode, modes )
		delete mode;

	foreach( Operation* const op, undoOperations )
		delete op;

	foreach( Operation* const op, redoOperations )
		delete op;

	// Make sure to delete viewport explicitly since it holds some 
	// reference-counting pointers, and if they are not destroyed
	// they will make some things try to call OpenGL functions
	// after the window context is already destroyed.
	editorScene.reset();
	delete viewport;
	delete engine;
}

//-----------------------------------//

void EditorFrame::initEngine()
{
	engine = new vapor::Engine(VAPOR_EDITOR_NAME, nullptr, false);
	engine->init( false );

	viewport = new Viewport( engine, this );
	
	vaporControl* control = viewport->getControl();
	
	control->onRender += fd::bind( &EditorFrame::onRender, this );
	control->onUpdate += fd::bind( &EditorFrame::onUpdate, this );

	engine->getRenderDevice()->init();
	engine->getVFS()->mountDefaultLocations();

	TaskManager* tm = engine->getTaskManager();
	tm->onTaskEvent += fd::bind( &EditorFrame::onTaskEvent, this );

	// Register all the mouse events.
	Mouse* const mouse = engine->getInputManager()->getMouse();
	mouse->onMouseMove += fd::bind( &EditorFrame::onMouseMove, this );
	mouse->onMouseDrag += fd::bind( &EditorFrame::onMouseDrag, this );
	mouse->onMouseButtonPress += fd::bind( &EditorFrame::onMousePress, this );
	mouse->onMouseButtonRelease += fd::bind( &EditorFrame::onMouseRelease, this );
	mouse->onMouseEnter	+= fd::bind( &EditorFrame::onMouseEnter, this );
	mouse->onMouseExit += fd::bind( &EditorFrame::onMouseLeave, this );
}

//-----------------------------------//

void EditorFrame::createModes()
{
	modes.push_back( new GizmoMode(this) );
	modes.push_back( new TerrainMode(this) );

	// TODO: hack
	onModeSwitch( modes.front(), 18237 );
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
	
	sceneTreeCtrl = new SceneTreeCtrl(engine, panelScene, ID_SceneTree,
		wxDefaultPosition, wxSize(230, -1) );

	sceneTreeCtrl->onItemSelected += fd::bind( &EditorFrame::OnNodeSelected, this );

	panelSceneSizer->Add( sceneTreeCtrl, 1, wxALL|wxEXPAND, 0 );

	panelScene->SetSizerAndFit( panelSceneSizer );

	int scenePage = notebookCtrl->AddPage( panelScene, wxT("Scene"), true, 0 );

	sizer->Add(notebookCtrl, 0, wxALL|wxEXPAND, 0 );

	//-----------------------------------//

	wxPanel* panelResources = new wxPanel( notebookCtrl, wxID_ANY );
	
	wxBoxSizer* panelResourcesSizer = new wxBoxSizer( wxVERTICAL );
	
	resourceTreeCtrl = new ResourceTreeCtrl(engine, panelResources, ID_SceneTree,
		wxDefaultPosition, wxSize(220, -1) );

	panelResourcesSizer->Add( resourceTreeCtrl, 1, wxALL|wxEXPAND, 0 );

	panelResources->SetSizerAndFit( panelResourcesSizer );

	notebookCtrl->AddPage( panelResources, wxT("Resources"), true, 1 );

	notebookCtrl->SetSelection( scenePage );

	//-----------------------------------//

	terrainPage = new TerrainPage( engine, notebookCtrl );
	notebookCtrl->AddPage( terrainPage, "Terrains", true, 2 );

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

	// --------------
	// Project tools
	// --------------

	toolBar->AddTool( wxID_ANY, "New", wxMEMORY_BITMAP(page_empty) );
	toolBar->AddTool( wxID_ANY, "Open", wxMEMORY_BITMAP(folder_explore) ); 
	toolBar->AddTool( Toolbar_Save, "Save", wxMEMORY_BITMAP(disk) );
	
	toolBar->AddSeparator();

	toolBar->AddTool( Toolbar_Undo, "Undo", wxMEMORY_BITMAP(arrow_undo) );
	toolBar->AddTool( Toolbar_Redo, "Redo", wxMEMORY_BITMAP(arrow_redo) );

	updateUndoRedoUI();

	toolBar->AddSeparator();

	toolBar->AddTool( Toolbar_ToogleConsole, "Console", 
		wxMEMORY_BITMAP(application_xp_terminal), "Open/close the scripting console" ); 

	toolBar->AddTool( Toolbar_ToogleGrid, "Grid", 
		wxMEMORY_BITMAP(grid_icon), "Show/hide the editor grid", wxITEM_CHECK );

	toolBar->ToggleTool( Toolbar_ToogleGrid, true );

	toolBar->AddTool( Toolbar_TooglePlay, "Play", wxMEMORY_BITMAP(resultset_next), 
		"Enable/disable Play mode", wxITEM_CHECK );

	// --------------------
	// Mode-specific tools
	// --------------------

	foreach( Mode* const mode, modes )
	{
		mode->onModeInit( toolBar, modesMap );
	}

	// --------------
	// UI tools
	// --------------

	toolBar->AddSeparator();

	toolBar->AddTool( Toolbar_ToogleSidebar, "Show/hide sidebar", 
		wxMEMORY_BITMAP(application_side_tree_right), "Shows/hides the sidebar" ); 

	codeEvaluator = new ConsoleFrame( engine, this, "Scripting Console" );
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