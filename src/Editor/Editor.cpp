/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "Editor.h"
#include "ArtProvider.h"
#include "toolbar_icons.h"
#include "icons.h"

#define VAPOR_USE_NAMESPACES
#include <vapor/Headers.h>

#include "vapor/render/Quad.h"

namespace vapor { namespace editor {

//-----------------------------------//

using namespace vapor;
using namespace vapor::math;
using namespace vapor::render;
using namespace vapor::resources;
using namespace vapor::scene;

//-----------------------------------//

// ----------------------------------------------------------------------------
// event tables and other macros for wxWidgets
// ----------------------------------------------------------------------------

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them. It can be also done at run-time, but for the
// simple menu events like this the static method is much simpler.
BEGIN_EVENT_TABLE(EditorFrame, wxFrame)
    EVT_MENU(Editor_Quit,  EditorFrame::OnQuit)
    EVT_MENU(Editor_About, EditorFrame::OnAbout)
	EVT_MENU(Toolbar_ToogleConsole, EditorFrame::OnToolbarButtonClick)
	//EVT_COMMAND(Toolbar_ToggleScene, EditorFrame::OnToolbarButtonClick)
END_EVENT_TABLE()

//-----------------------------------//

// Create a new application object: this macro will allow wxWidgets to create
// the application object during program execution (it's better than using a
// static object for many reasons) and also implements the accessor function
// wxGetApp() which will return the reference of the right type.
IMPLEMENT_APP(EditorApp)

//-----------------------------------//

// 'Main program' equivalent: the program execution "starts" here
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

	// add a new art provider for stock icons
	//wxArtProvider::Push(new ArtProvider);

    // Create the main application window, provide a default size and show it.
	// Unlike simple controls, frames are not shown automatically when created.
    EditorFrame* frame = new EditorFrame("vaporEditor - level editor for vapor");
	frame->SetSize(750, 500);
    frame->Show(true);

	// Set our created window as the top-level window. This provides a few
	// advantages. The wxWidgets manual says the first created frame will
	// be the default top-level window, but better be safe than sorry. :)
	SetTopWindow( frame );

    // success: wxApp::OnRun() will be called which will enter the main message
    // loop and the application will run. If we returned false here, the
    // application would exit immediately.
    return true;
}

//-----------------------------------//

// frame constructor
EditorFrame::EditorFrame(const wxString& title)
       : wxFrame(nullptr, wxID_ANY, title), engine( nullptr )
{
    // set the frame icon
    SetIcon( wxIcon( "editor" ) );

	sizer = new wxBoxSizer( wxHORIZONTAL );

	// initialize the engine
	initEngine();	

	// create window basic widgets
	createMenus();
	createToolbar();
	createStatusbar();
	createNotebook();

	SetSizerAndFit( sizer );

	codeEvaluator = new ConsoleFrame( engine, this, "Scripting Console" );

	createScene();
}

//-----------------------------------//

EditorFrame::~EditorFrame()
{
	vaporCtrl->Destroy();
	delete engine;
}

//-----------------------------------//

void EditorFrame::initEngine()
{
	engine = new vapor::Engine("vaporEditor", nullptr, false);
	engine->init( false );

	vaporCtrl = new vaporControl(engine, this);

	// add a new vaporControl in the frame
	sizer->Add( vaporCtrl, 1, wxALL|wxEXPAND, 0 );

	engine->getRenderDevice()->init();
	engine->setupInput();
}

//-----------------------------------//

void EditorFrame::createScene()
{
	engine->getVFS()->mount( "media" );

	ScenePtr scene = engine->getSceneManager();
	ResourceManager* rm = engine->getResourceManager();

	ProgramPtr diffuse( new GLSL_Program( 
			rm->loadResource< GLSL_Shader >( "diffuse.vs" ),
			rm->loadResource< GLSL_Shader >( "diffuse.fs" ) ) );

	ProgramPtr tex( new GLSL_Program( 
			rm->loadResource< GLSL_Shader >( "tex.vs" ),
			rm->loadResource< GLSL_Shader >( "tex.fs" ) ) );

	// Create a new Camera
	NodePtr camera( new Node( "MainCamera" ) );
	CameraPtr cam( new FirstPersonCamera( engine->getInputManager(), engine->getRenderDevice() ) );
	camera->addComponent( TransformPtr( new Transform() ) );
	camera->addComponent( cam );
	scene->add( camera );

	MaterialPtr mat( new Material( "GridMaterial", diffuse ) );
	NodePtr grid( new Node( "EditorGrid" ) );
	grid->addComponent( TransformPtr( new Transform() ) );
	grid->addComponent( ComponentPtr( new Grid( mat ) ) );
	scene->add( grid );

	foreach( const RenderablePtr& rend, grid->getComponent<Geometry>("Grid")->getRenderables() )
	{
		rend->getMaterial()->setProgram( diffuse );
	}

	ScriptPtr lua = rm->loadResource< Script >( "teste.lua" );
	engine->getScriptState()->registerScript( lua );

	MS3DPtr mesh = rm->loadResource< MS3D >( "terreno.ms3d" );

	foreach( const RenderablePtr& rend, mesh->getRenderables() )
	{
		rend->getMaterial()->setProgram( tex );
	}

	NodePtr terreno( new Node( "Terreno" ) );
	terreno->addComponent( TransformPtr( new Transform() ) );
	terreno->addComponent( mesh );
	terreno->getTransform()->scale( 0.3f );
	scene->add( terreno );
}

//-----------------------------------//

void EditorFrame::createNotebook()
{
	notebookCtrl = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );

	//-----------------------------------//

	wxPanel* panelScene = new wxPanel( notebookCtrl, wxID_ANY, wxDefaultPosition, 
		wxDefaultSize, wxTAB_TRAVERSAL );
	
	wxBoxSizer* panelSceneSizer = new wxBoxSizer( wxVERTICAL );
	
	sceneTreeCtrl = new SceneTreeCtrl(engine, panelScene, ID_SceneTree,
		wxDefaultPosition, wxSize(220, -1) );

	panelSceneSizer->Add( sceneTreeCtrl, 1, wxALL|wxEXPAND, 0 );

	panelScene->SetSizerAndFit( panelSceneSizer );

	notebookCtrl->AddPage( panelScene, wxT("Scene"), true );

	sizer->Add(notebookCtrl, 0, wxALL|wxEXPAND, 0 );

	//-----------------------------------//

	wxPanel* panelResources = new wxPanel( notebookCtrl, wxID_ANY, wxDefaultPosition, 
		wxDefaultSize, wxTAB_TRAVERSAL );
	
	wxBoxSizer* panelResourcesSizer = new wxBoxSizer( wxVERTICAL );
	
	resourceTreeCtrl = new ResourceTreeCtrl(engine, panelResources, ID_SceneTree,
		wxDefaultPosition, wxSize(220, -1) );

	panelResourcesSizer->Add( resourceTreeCtrl, 1, wxALL|wxEXPAND, 0 );

	panelResources->SetSizerAndFit( panelResourcesSizer );

	notebookCtrl->AddPage( panelResources, wxT("Resources"), true );
}

//-----------------------------------//

void EditorFrame::createStatusbar()
{
#if wxUSE_STATUSBAR
    // create a status bar just for fun (by default with 1 pane only)
    CreateStatusBar(2);
	SetStatusText("vaporEngine (FPS: OVER 9000!)");
#endif // wxUSE_STATUSBAR
}

//-----------------------------------//

void EditorFrame::createMenus()
{
#if wxUSE_MENUS
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
#endif // wxUSE_MENUS
}

//-----------------------------------//

void EditorFrame::createToolbar()
{
	wxToolBar* toolBar = this->CreateToolBar( wxTB_HORIZONTAL, wxID_ANY );
	
	toolBar->AddTool( wxID_ANY, "New", wxMEMORY_BITMAP(page) );
	toolBar->AddTool( wxID_ANY, "Open", wxMEMORY_BITMAP(folder_explore) ); 
	toolBar->AddTool( wxID_ANY, "Save", wxMEMORY_BITMAP(disk) );
	
	toolBar->AddSeparator();

	toolBar->AddTool( Toolbar_ToogleConsole, "Open Console scripting", 
		wxMEMORY_BITMAP(application_xp_terminal) ); 

	toolBar->AddSeparator();

	toolBar->AddTool( wxID_ANY, "Play", wxMEMORY_BITMAP(resultset_next) );

	toolBar->Realize();	

	// connect events
	//Bind(wxEVT_COMMAND_TOOL_CLICKED, &EditorFrame::OnToolbarButtonClick, wxID_ANY );
}

//-----------------------------------//
// event handlers
//-----------------------------------//

void EditorFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    // true is to force the frame to close
    Close(true);
}

//-----------------------------------//

void EditorFrame::OnToolbarButtonClick(wxCommandEvent& event)
{
	if( event.GetId() == Toolbar_ToogleConsole )
	{
		codeEvaluator->Show( !codeEvaluator->IsShown() );
		codeEvaluator->SetFocus();
	}
	//if( event.GetId() == Toolbar_ToggleScene )
	//{
	//	if( sceneTreeCtrl->IsShown() )	
	//		//treeCtrl->HideWithEffect(wxSHOW_EFFECT_SLIDE_TO_LEFT, 250);
	//		treeCtrl->Hide();
	//	else
	//		treeCtrl->Show();
	//		//treeCtrl->ShowWithEffect(wxSHOW_EFFECT_SLIDE_TO_RIGHT, 250);

	//	Update();
	//}

	//event.Skip();
}

//-----------------------------------//

void EditorFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox(
		wxString::Format( 
			"Welcome to %s!\n\n"
			"This is vaporEditor\n"
			"running under %s.",
			wxVERSION_STRING,
			wxGetOsDescription() ),
		"About vaporEditor",
		wxOK | wxICON_INFORMATION,
		this);
}

//-----------------------------------//

} } // end namespaces