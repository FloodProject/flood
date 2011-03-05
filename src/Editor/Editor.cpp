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
	//wxHandleFatalExceptions();

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
	, paneCtrl(nullptr)
	, toolbarCtrl(nullptr)
	, notebookCtrl(nullptr)
	, eventManager(nullptr)
	, pluginManager(nullptr)
	, pluginManagerFrame(nullptr)
	, currentDocument(nullptr)
{
	editorInstance = this;

	createUI();
	createEngine();
	createServices();
	createPlugins();
	createToolbar();
	createLastUI();

#if 0
	wxCommandEvent event;
	onNewButtonClick(event);
#endif
}

//-----------------------------------//

EditorFrame::~EditorFrame()
{
	paneCtrl->UnInit();
	delete paneCtrl;

 	delete eventManager;
	delete pluginManager;

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
	//PLUGIN(Gizmo);
	//PLUGIN(Terrain);
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
}

//-----------------------------------//

void EditorFrame::createEngine()
{
	engine = new Engine;
	engine->create(VAPOR_EDITOR_NAME);
	engine->init(false);
	
	engine->setupInput();

	// Mount the editor default media directories.
	FileSystem* fs = engine->getFileSystem();
	fs->mountDefaultLocations();
}

//-----------------------------------//

void EditorFrame::createServices()
{
	pluginManager = new PluginManager(this);
	eventManager = new Events(this);
}

//-----------------------------------//

void EditorFrame::createUI()
{
    SetIcon( wxIcon("editor") );

	paneCtrl = new wxAuiManager;
	paneCtrl->SetManagedWindow(this);

	// Create notebook
	notebookCtrl = new wxAuiNotebook(this);
	notebookCtrl->Bind(wxEVT_COMMAND_AUINOTEBOOK_PAGE_CHANGED, &EditorFrame::onNotebookPageChanged, this);
	notebookCtrl->Bind(wxEVT_COMMAND_AUINOTEBOOK_PAGE_CLOSE, &EditorFrame::onNotebookPageClose, this);
	
	wxAuiPaneInfo pane; pane.CenterPane();
	paneCtrl->AddPane(notebookCtrl, pane );

	// Create menus
	createMenus();

	// Create toolbar
	int style = wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_OVERFLOW;
	toolbarCtrl = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, style);
	toolbarCtrl->Bind(wxEVT_COMMAND_MENU_SELECTED, &EditorFrame::OnToolbarButtonClick, this);
}

//-----------------------------------//

void EditorFrame::createToolbar()
{
	if(!toolbarCtrl) return;

	toolbarCtrl->AddTool( Toolbar_ToogleGrid, "Grid", 
		wxMEMORY_BITMAP(grid_icon), "Show/hide the editor grid", wxITEM_CHECK );

	toolbarCtrl->AddTool( Toolbar_TooglePhysicsDebug, "Physics", 
		wxMEMORY_BITMAP(grid_icon), "Show/hide the physics debug", wxITEM_CHECK );

	toolbarCtrl->ToggleTool( Toolbar_ToogleGrid, true );

	toolbarCtrl->AddTool( Toolbar_TooglePlay, "Play", wxMEMORY_BITMAP(resultset_next), 
		"Enable/disable Play mode", wxITEM_CHECK );

	toolbarCtrl->AddSeparator();

	toolbarCtrl->AddTool( Toolbar_ToogleViewport, "Toogles maximize view", 
		wxMEMORY_BITMAP(application_split), "Toogles maximize view" );

	toolbarCtrl->AddTool( Toolbar_ToogleSidebar, "Shows/hides the sidebar", 
		wxMEMORY_BITMAP(application_side_tree_right), "Shows/hides the sidebar" );

	toolbarCtrl->AddTool( Toolbar_TooglePlugin, "Shows/hides the plugin manager", 
		wxMEMORY_BITMAP(cog), "Shows/hides the plugin manager" );

	getAUI()->AddPane(toolbarCtrl, wxAuiPaneInfo().ToolbarPane().Caption("Toolbar").Top());
}


//-----------------------------------//

void EditorFrame::redrawView()
{
#if 0
	if( !viewframe ) return;
	viewframe->flagRedraw();
#endif
}

//-----------------------------------//

Document* EditorFrame::getDocumentFromPage(int selection)
{
	if( selection < 0 ) return nullptr;

	wxWindow* window = notebookCtrl->GetPage(selection);

	for(uint i = 0; i < documents.size(); i++)
	{
		Document* document = documents[i];
		
		if( document->getWindow() == window )
			return document;
	}

	return nullptr;
}

//-----------------------------------//

void EditorFrame::onNotebookPageChanged(wxAuiNotebookEvent& event)
{
	Document* oldDocument = getDocumentFromPage( event.GetOldSelection() );
	Document* newDocument = getDocumentFromPage( event.GetSelection() );

	if(oldDocument)
	{
		oldDocument->onDocumentUnselect();
		eventManager->onDocumentUnselect(*oldDocument);
		currentDocument = nullptr;
	}

	if(newDocument)
	{
		currentDocument = newDocument;
		newDocument->onDocumentSelect();
		eventManager->onDocumentSelect(*newDocument);
	}
}

//-----------------------------------//

void EditorFrame::onNotebookPageClose(wxAuiNotebookEvent& event)
{
	Document* document = getDocumentFromPage( event.GetSelection() );

	std::vector<Document*>::iterator it =
		std::find(documents.begin(), documents.end(), document);

	if( it != documents.end() )
	{
		document->onDocumentUnselect();
		eventManager->onDocumentUnselect(*document);

		documents.erase(it);
	}

	if( currentDocument == document )
	{
		currentDocument = nullptr;
	}
}

//-----------------------------------//

void EditorFrame::addDocument(Document* document)
{
	if( !document ) return;

	documents.push_back(document);

	wxWindow* window = document->getWindow();
	notebookCtrl->AddPage( window, document->getName() );

	getAUI()->Update();
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
#if 0
		const EntityPtr& grid = editorScene->findEntity("Grid");
		
		if( grid )
			grid->setVisible( !grid->isVisible() );
		
		redrawView();
#endif	
		break;
	}
	//-----------------------------------//
	case Toolbar_TooglePhysicsDebug:
	{
#ifdef VAPOR_PHYSICS_BULLET
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
#if 0
		bool switchToPlay = event.IsChecked();
		switchPlayMode(switchToPlay);
#endif
		break;
	}
	//-----------------------------------//
	case Toolbar_ToogleViewport:
	{
#if 0
		int curExpansion = viewSplitter->GetExpanded();

		if( curExpansion >= 0 )
			viewSplitter->SetExpanded(-1);
		else
			viewSplitter->SetExpanded(0);
#endif
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


#if 0
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
#endif