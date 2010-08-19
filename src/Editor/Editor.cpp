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
#include "plugins/Resources/ResourcesPlugin.h"
#include "plugins/Property/PropertyPlugin.h"
#include "plugins/Gizmos/GizmoPlugin.h"
#include "plugins/Terrain/TerrainPlugin.h"
#include "plugins/Camera/CameraPlugin.h"
#include "plugins/Sample/SamplePlugin.h"

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
        return false;

	wxHandleFatalExceptions(true);

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

bool EditorApp::OnExceptionInMainLoop()
{
	throw;
}

//-----------------------------------//

void EditorApp::OnUnhandledException()
{
	throw;
}

//-----------------------------------//

EditorFrame::EditorFrame(const wxString& title)
	: wxFrame(nullptr, wxID_ANY, title)
	, engine(nullptr)
	, mainSplitter(nullptr)
	, viewframe(nullptr)
	, undoManager(nullptr)
	, eventManager(nullptr)
	, pluginManagerFrame(nullptr)
{
    // Set the editor icon.
    SetIcon( wxIcon("editor") );
	
	// Initialize the engine.
	initEngine();

	createMenus();
	createToolbar();
	createNotebook();

	pluginManager = new PluginManager(this);
	eventManager = new Events(this);
	undoManager = new UndoManager();

	createPlugins();

	createScene();
	createEditorScene();

	toolBar->Realize();
	SetSizerAndFit( sizer );

	waitFinishLoad();

	//SetFocusIgnoringChildren();
}

//-----------------------------------//

EditorFrame::~EditorFrame()
{
	delete eventManager;
	delete pluginManager;
	delete undoManager;
	delete mainSplitter;

	// Stop the frame events.
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
	onUpdate( 0 );

	RenderControl* control = viewframe->getControl();
	control->startFrameLoop();
}

//-----------------------------------//

void EditorFrame::createMainViewframe()
{
	mainSplitter = new wxFourWaySplitter(this);
	viewframe = new Viewframe( mainSplitter );

	sizer = new wxBoxSizer( wxHORIZONTAL );
	sizer->Add( mainSplitter, 1, wxEXPAND|wxALL );

	mainSplitter->SetWindow( 0, viewframe );
	mainSplitter->SetExpanded( viewframe );

	RenderControl* control = viewframe->getControl();
	control->onRender += fd::bind( &EditorFrame::onRender, this );
	control->onUpdate += fd::bind( &EditorFrame::onUpdate, this );
	control->SetFocus();

	RenderDevice* device = engine->getRenderDevice();
	Window* window = (Window*) control->getRenderWindow(); 

	device->setWindow( window );
	device->setRenderTarget( window );

	engine->setupInput();
	inputManager = control->getInputManager();

	NodePtr camera( createCamera() );	
	editorScene->add( camera );

	Viewport* viewport = viewframe->createViewport( camera );
	viewport->setClearColor( Color(0.0f, 0.10f, 0.25f) );

	TransformPtr transform( camera->getTransform() );
	transform->translate( 0, 20, -65 );
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
	
	Frustum& frustum = camera->getFrustum();
	frustum.farPlane = 100000;

	// Generate a new unique name.
	std::string name( "EditorCamera"+String::fromNumber(i++) );

	NodePtr nodeCamera( new Node(name) );
	nodeCamera->addTransform();
	nodeCamera->addComponent( camera );
	nodeCamera->setTag( Tags::NonPickable, true );
	nodeCamera->setTag( EditorTags::EditorOnly, true );

	return nodeCamera;
}

//-----------------------------------//

void EditorFrame::createNotebook()
{
	const wxSize notebookMinSize(220, wxSIZE_AUTO_HEIGHT);
	notebookCtrl = new wxNotebook( this, wxID_ANY );
	notebookCtrl->SetMinSize(notebookMinSize);
	sizer->Add(notebookCtrl, 0, wxALL|wxEXPAND, 0 );

	wxImageList* img = new wxImageList(16, 16);
	notebookCtrl->AssignImageList( img );
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

	toolBar->AddTool( Toolbar_ToogleViewport, "Toogles maximize viewport", 
		wxMEMORY_BITMAP(scale), "Toogles maximize viewport" );

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
	
	#pragma TODO("Renderables need to be sent in a single queue")

	camera->setViewport( viewport );
	camera->render( engine->getSceneManager() );
	camera->render( editorScene, false );
}

//-----------------------------------//

void EditorFrame::onUpdate( double delta )
{
	editorScene->update( delta );
	engine->update( delta );

	//eventManager->onSceneUpdate();
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
		//PhysicsManager* pm = engine->getPhysicsManager();
		//if( pm ) pm->setSimulationEnabled( !pm->getSimulationEnabled() );
		return;
	}
	//-----------------------------------//
	case Toolbar_ToogleViewport:
	{
		int curExpansion = mainSplitter->GetExpanded();

		if( curExpansion >= 0 )
			mainSplitter->SetExpanded(-1);
		else
			mainSplitter->SetExpanded(0);
		
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
	//-----------------------------------//
	} // end switch
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
	rm->loadResource("ProjTex.glsl");

	// Sky.
	ImagePtr clouds = rm->loadResource<Image>( "noise2.png" );

	SkydomePtr skydome( new Skydome() );
	skydome->setClouds( clouds );
	
	NodePtr nodeSky( new Node("Sky") );
	nodeSky->addTransform();	
	nodeSky->addComponent( skydome );
	scene->add( nodeSky );

	TransformPtr transSky = nodeSky->getTransform();
	transSky->scale(2);

	// Water.
	MaterialPtr matWater( new Material("WaterMat", "Water") );
	matWater->setTexture( 0, "water.png" );
	matWater->setBlending(BlendingSource::SourceAlpha, BlendingDestination::InverseSourceAlpha);
	WaterPtr water( new Water(matWater) );

	NodePtr nodeWater( new Node("Water") );
	nodeWater->addTransform();	
	nodeWater->addComponent( water );
	//scene->add( nodeWater );

	TransformPtr transWater = nodeWater->getTransform();
	transWater->rotate(90, 0, 0);
	transWater->scale(10);
	transWater->translate(0, 5, 0);

	// Light.
	LightPtr light;
	light.reset( new Light(LightType::Directional) );
	light->setDiffuseColor( Color::Red );
	light->setAmbientColor( Color::Yellow );

	TransformPtr transLight( new Transform(0, 100, 0) );
	transLight->rotate(45, 0, 0);

	NodePtr nodeLight( new Node("Light") );
	nodeLight->addComponent( transLight );
	nodeLight->addComponent( light );
	scene->add( nodeLight );

	// Terrain.
	MaterialPtr cellMaterial( new Material("CellMaterial") );
	cellMaterial->setTexture( 0, "sand.png" );
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
	terrain->addCell( 0, 0, heightMap );

	// Wait until all resources are loaded.
	rm->waitUntilQueuedResourcesLoad();

	engine->update( 0.1f );
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

} } // end namespaces