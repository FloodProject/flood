/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "PCH.h"
#include "Editor.h"
#include "EditorIcons.h"

using namespace vapor::input;

namespace vapor { namespace editor {

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// IDs for the controls and the menu commands
enum
{
	Camera_Viewport,

    // menu items
    Editor_Quit = wxID_EXIT,

    // it is important for the id corresponding to the "About" command to have
    // this standard value as otherwise it won't be handled properly under Mac
    // (where it is special and put into the "Apple" menu)
    Editor_About = wxID_ABOUT,

	// Toolbar buttons
	Toolbar_ToggleScene,
	Toolbar_ToogleConsole,
	Toolbar_ToogleGrid,
	Toolbar_TooglePlay,
};

// ----------------------------------------------------------------------------
// event tables and other macros for wxWidgets
// ----------------------------------------------------------------------------

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them. It can be also done at run-time, but for the
// simple menu events like this the static method is much simpler.
BEGIN_EVENT_TABLE(EditorFrame, wxFrame)
    EVT_MENU(Editor_Quit,  EditorFrame::OnQuit)
    EVT_MENU(Editor_About, EditorFrame::OnAbout)
	EVT_MENU(wxID_ANY, EditorFrame::OnToolbarButtonClick)
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
	createScene();

	SetSizerAndFit( sizer );
	vaporCtrl->SetFocus();
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

	viewport = new Viewport( engine, this );
	sizer->Add( viewport, 1, wxEXPAND|wxALL );

	engine->getRenderDevice()->init();

	vaporCtrl = viewport->vaporCtrl;
 
	//engine->getPhysicsManager()->createWorld();
	//engine->getPhysicsManager()->setSimulationEnabled( false );
	
	input::Mouse* mouse = engine->getInputManager()->getMouse();
	mouse->onMouseButtonPress += fd::bind( &EditorFrame::onMouseClick, this );
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

	ProgramManager::getInstance().registerProgram( "diffuse", diffuse );
	ProgramManager::getInstance().registerProgram( "tex", tex );

	MaterialPtr mat( new Material( "GridMaterial", diffuse ) );
	NodePtr grid( new Node( "EditorGrid" ) );
	grid->addComponent( TransformPtr( new Transform() ) );
	grid->addComponent( ComponentPtr( new Grid( mat ) ) );
	scene->add( grid );

	MeshPtr mesh = rm->loadResource<Mesh>( "cubo.ms3d" );

	foreach( const RenderablePtr& rend, mesh->getGeometry()->getRenderables() )
	{
		rend->getMaterial()->setProgram( tex );
	}

	NodePtr ct( new Node( "ct" ) );
	ct->addComponent( TransformPtr( new Transform( 0.0f, 50.0f, 0.0f ) ) );
	ct->addComponent( mesh->getGeometry() );
	scene->add(ct);

	//TerrainSettings settings;
	//settings.CellSize = 512;
	//settings.TileDimensions = 32;
	//settings.MaxHeight = 100;

	//TerrainPtr terrain( new Terrain( settings ) );

	//NodePtr terreno( new Node( "Terreno" ) );
	//terreno->addComponent( TransformPtr( new Transform() ) );
	//terreno->addComponent( terrain );
	////scene->add( terreno );

	//ImagePtr heightmap = rm->loadResource< Image >( "height2.png" );
	//Cell* cell = terrain->createCell( heightmap, 0, 0 );
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

	int scenePage = notebookCtrl->AddPage( panelScene, wxT("Scene"), true );

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

	notebookCtrl->SetSelection( scenePage );
}

//-----------------------------------//

void EditorFrame::createStatusbar()
{
#if wxUSE_STATUSBAR
    // create a status bar just for fun (by default with 1 pane only)
    //CreateStatusBar(2);
	//SetStatusText("vaporEngine (FPS: OVER 9000!)");
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

	// --------------
	// Project tools
	// --------------

	toolBar->AddTool( wxID_ANY, "New", wxMEMORY_BITMAP(page_empty) );
	toolBar->AddTool( wxID_ANY, "Open", wxMEMORY_BITMAP(folder) ); 
	toolBar->AddTool( wxID_ANY, "Save", wxMEMORY_BITMAP(disk) );
	
	toolBar->AddSeparator();

	toolBar->AddTool( wxID_ANY, "Undo", wxMEMORY_BITMAP(arrow_undo) );
	toolBar->AddTool( wxID_ANY, "Redo", wxMEMORY_BITMAP(arrow_redo) );

	toolBar->AddSeparator();

	toolBar->AddTool( Toolbar_ToogleConsole, "Console", 
		wxMEMORY_BITMAP(application_xp_terminal), "Open/close the scripting console"/*, wxITEM_CHECK*/ ); 

	toolBar->AddTool( Toolbar_ToogleGrid, "Grid", 
		wxMEMORY_BITMAP(grid_icon), "Show/hide the editor grid", wxITEM_CHECK );

	toolBar->ToggleTool( Toolbar_ToogleGrid, true );

	toolBar->AddTool( Toolbar_TooglePlay, "Play", wxMEMORY_BITMAP(resultset_next), 
		"Enable/disable Play mode", wxITEM_CHECK );

	// --------------
	// Gizmo tools
	// --------------

	toolBar->AddSeparator();

	toolBar->AddTool( wxID_ANY, "Select", wxMEMORY_BITMAP(cursor), 
		"Selects the Entity Selection tool", wxITEM_RADIO );

	toolBar->AddTool( wxID_ANY, "Move", wxMEMORY_BITMAP(move), 
		"Selects the Move tool", wxITEM_RADIO );

	toolBar->AddTool( wxID_ANY, "Rotate", wxMEMORY_BITMAP(rotate2), 
		"Selects the Rotate tool", wxITEM_RADIO );

	toolBar->AddTool( wxID_ANY, "Scale", wxMEMORY_BITMAP(scale), 
		"Selects the Scale tool", wxITEM_RADIO );
	
	// --------------
	// Terrain tools
	// --------------

	toolBar->AddTool( wxID_ANY, "Raise/Lower", wxMEMORY_BITMAP(terrain_raise_lower), 
		"Raises/Lowers the terrain", wxITEM_RADIO );

	toolBar->AddTool( wxID_ANY, "Paint", wxMEMORY_BITMAP(terrain_paint),
		"Paints the terrain", wxITEM_RADIO );

	//toolBar->AddTool( wxID_ANY, "", wxMEMORY_BITMAP(resultset_next) );

	toolBar->Realize();	

	// connect events
	//Bind(wxEVT_COMMAND_TOOL_CLICKED, &EditorFrame::OnToolbarButtonClick, wxID_ANY );

	codeEvaluator = new ConsoleFrame( engine, this, "Scripting Console" );
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
	switch( event.GetId() ) 
	{
		case Toolbar_ToogleConsole:
		{
			codeEvaluator->Show( !codeEvaluator->IsShown() );
			codeEvaluator->SetFocus();
			break;
		}
		
		case Toolbar_ToogleGrid:
		{
			NodePtr grid = engine->getSceneManager()->getEntity( "EditorGrid" );
			if( grid ) grid->setVisible( !grid->getVisible() );
			break;
		}

		case Toolbar_TooglePlay:
		{
			physics::PhysicsManager* pm = engine->getPhysicsManager();
			if( pm ) pm->setSimulationEnabled( !pm->getSimulationEnabled() );
		}
	}
}

//-----------------------------------//

void EditorFrame::onMouseClick( const MouseButtonEvent& mbe )
{
	// Let's do ray picking...
	// Based on: http://www.mvps.org/directx/articles/rayproj.htm

	//if( vaporCtrl->HasFocus() ) return;

	// Disable all enabled bounding boxes.
	foreach( NodePtr node, selectedNodes )
	{
		foreach( GeometryPtr geometry, node->getGeometry() )
		{
			//geometry->setBoundingBoxVisible( false );
		}
	}

	selectedNodes.clear();

	// Just get all the needed classes with the data for picking.
	Ray pickRay = viewport->camera->getRay( mbe.x, mbe.y );

	//std::vector< Vector3 > vertex;
	//vertex.push_back( pNear );
	//vertex.push_back( pFar );

	//std::vector< Vector3 > colors;
	//colors.push_back( Vector3( 1.0f, 0.0f, 0.0f ) );
	//colors.push_back( Vector3( 1.0f, 0.0f, 0.0f ) );

	//VertexBufferPtr vb( new VertexBuffer() );
	//vb->set( VertexAttribute::Vertex, vertex );
	//vb->set( VertexAttribute::Color, colors );

	//RenderablePtr rend( new Renderable( Primitive::Lines, vb ) );
	//MaterialPtr mat( new Material( "LineMaterial", "diffuse" ) );
	//rend->setMaterial( mat );
	//GeometryPtr geom( new Geometry(rend) );
	//NodePtr line( new Node( "line" ) );
	//line->addComponent( TransformPtr( new Transform() ) );
	//line->addComponent( geom );
	//scene->add( line );

	ScenePtr scene = engine->getSceneManager();

	// Do some ray tracing to find a collision.
	foreach( NodePtr node, scene->getChildren() )
	{
		//if( node->getName() == "line" ) continue;

		foreach( GeometryPtr geometry, node->getGeometry() )
		{
			//const AABB& aabb = geometry->getBoundingBox();
			
			//float distance;
			//if( aabb.intersects( pickRay, distance ) )
			//{
			//	// We found what we want, enable its bounding box.
			//	geometry->setBoundingBoxVisible( true );
			//	selectedNodes.push_back( node );
			//	debug( "distance: %f", distance );
			//	goto pickDone;
			//}
		}
	}

pickDone:

	return;
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
