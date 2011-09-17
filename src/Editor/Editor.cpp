/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"
#include "Editor.h"
#include "EditorIcons.h"
#include "EditorTags.h"
#include "Settings.h"

#include "Core/PluginManager.h"
#include "Core/Utilities.h"

#include "Viewframe.h"
#include "RenderControl.h"
#include "EditorInputManager.h"

#include "EventManager.h"
#include "UndoManager.h"
#include "DocumentManager.h"

#include <wx/debugrpt.h>

#include "Plugins/Project/ProjectPlugin.h"

#define CREATE_PROJECT_ON_STARTUP

//wxIMPLEMENT_WXWIN_MAIN_CONSOLE
wxIMPLEMENT_WXWIN_MAIN

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

wxIMPLEMENT_APP_NO_MAIN(EditorApp);

bool EditorApp::OnInit()
{
	if( !wxApp::OnInit() ) return false;

	wxImage::AddHandler( new wxPNGHandler() );

	EditorFrame* frame = new EditorFrame(VAPOR_EDITOR_NAME);
	frame->SetSize(800, 500);

	SetTopWindow(frame);
	frame->Show(true);

	return true;
}

//-----------------------------------//
#ifndef BUILD_DEBUG
void EditorApp::OnFatalException()
{
	wxDebugReport report;
	wxDebugReportPreviewStd preview;

	report.AddExceptionContext();
	report.AddExceptionDump();

	if ( preview.Show(report) )
		report.Process();
}
#endif
//-----------------------------------//

static EditorFrame* gs_EditorInstance = nullptr;;
EditorFrame& GetEditor() { return *gs_EditorInstance; }

//-----------------------------------//

EditorFrame::EditorFrame(const wxString& title)
	: wxFrame(nullptr, wxID_ANY, title)
	, engine(nullptr)
	, paneCtrl(nullptr)
	, toolbarCtrl(nullptr)
	, notebookCtrl(nullptr)
	, eventManager(nullptr)
	, pluginManager(nullptr)
	, documentManager(nullptr)
{
	gs_EditorInstance = this;

	createUI();
	createEngine();
	createPlugins();
	createToolbar();
	createLastUI();

	Bind(wxEVT_IDLE, &EditorFrame::OnIdle, this);

	wxKeyProfile* mainProfile = new wxKeyProfile();
	mainProfile->SetName("Main");
	//mainProfile->AttachRecursively(this);
	mainProfile->ImportMenuBarCmd( GetMenuBar() );
	mainProfile->Enable();

	keyProfiles.Add(mainProfile);
	keyProfiles.SetSelProfile(0);

#ifdef CREATE_PROJECT_ON_STARTUP
	wxCommandEvent event;
	ProjectPlugin* project = GetPlugin<ProjectPlugin>();
	project->onNewButtonClick(event);
#endif

	AllocatorDumpInfo();
}

//-----------------------------------//

EditorFrame::~EditorFrame()
{
	Deallocate(documentManager);

	eventManager->disconnectPluginListeners();
	Deallocate(pluginManager);
 	
	Deallocate(eventManager);

	notebookCtrl->Destroy();
	paneCtrl->DetachPane(notebookCtrl);
	
	paneCtrl->UnInit();
	delete paneCtrl;

	Deallocate(engine);
}

//-----------------------------------//

void EditorFrame::OnIdle(wxIdleEvent& event)
{
	GetEngine()->update();

	const std::vector<Plugin*> plugins = pluginManager->getPlugins();
	
	for( size_t i = 0; i < plugins.size(); i++ )
	{
		EditorPlugin* plugin = (EditorPlugin*) plugins[i];
		plugin->onPluginUpdate();
	}
}

//-----------------------------------//

void EditorFrame::createPlugins()
{
	documentManager = AllocateThis(DocumentManager);
	documentManager->onDocumentAdded.Connect(this, &EditorFrame::onDocumentAdded);
	documentManager->onDocumentRemoved.Connect(this, &EditorFrame::onDocumentRemoved);

	pluginManager = AllocateThis(PluginManager);
	eventManager = AllocateThis(EventManager);

	std::vector<Plugin*> plugins;

	// Find and instantiate plugins.
	ClassCreateChilds(ReflectionGetType(EditorPlugin), AllocatorGetThis(), plugins);
	Plugin::sortByPriority(plugins);
	pluginManager->registerPlugins(plugins);
}

//-----------------------------------//

void EditorFrame::createEngine()
{
	engine = AllocateThis(Engine);
	engine->init(false);
	engine->setupInput();

	// Mount the default assets path.
	ResourceManager* res = engine->getResourceManager();
	Archive* archive = ArchiveCreateVirtual( GetResourcesAllocator() );
	ArchiveMountDirectories(archive, MediaFolder, GetResourcesAllocator());
	res->setArchive(archive);
}

//-----------------------------------//

void EditorFrame::createUI()
{
    SetIcon( wxIcon("iconEditor") );
	
	paneCtrl = new wxAuiManager();
	paneCtrl->SetManagedWindow(this);

	// Create notebook
	notebookCtrl = new wxAuiNotebook(this);
	notebookCtrl->Bind(wxEVT_COMMAND_AUINOTEBOOK_PAGE_CHANGED, &EditorFrame::onNotebookPageChanged, this);
	notebookCtrl->Bind(wxEVT_COMMAND_AUINOTEBOOK_PAGE_CLOSE, &EditorFrame::onNotebookPageClose, this);
	
	wxAuiPaneInfo pane;
	pane.CenterPane().PaneBorder(false);
	paneCtrl->AddPane(notebookCtrl, pane);

	// Create menus
	createMenus();

	// Create toolbar
	int style = wxAUI_TB_DEFAULT_STYLE /*| wxAUI_TB_OVERFLOW*/;
	toolbarCtrl = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, style);
	toolbarCtrl->SetGripperVisible(false);
	toolbarCtrl->GetArtProvider()->SetElementSize(wxAUI_TBART_GRIPPER_SIZE, 0);
	toolbarCtrl->Bind(wxEVT_COMMAND_MENU_SELECTED, &EditorFrame::OnToolbarButtonClick, this);
}

//-----------------------------------//

void EditorFrame::createToolbar()
{
	if(!toolbarCtrl) return;

	toolbarCtrl->AddTool( Toolbar_ToogleGrid, "Grid", wxMEMORY_BITMAP(grid_icon), "Show/hide the editor grid", wxITEM_CHECK );
	//toolbarCtrl->AddTool( Toolbar_TooglePhysicsDebug, "Physics", wxMEMORY_BITMAP(grid_icon), "Show/hide the physics debug", wxITEM_CHECK );
	toolbarCtrl->ToggleTool( Toolbar_ToogleGrid, true );
	toolbarCtrl->AddTool( Toolbar_TooglePlay, "Play", wxMEMORY_BITMAP(resultset_next), "Enable/disable Play mode", wxITEM_CHECK );
	
	toolbarCtrl->AddSeparator();

	toolbarCtrl->AddTool( Toolbar_ToogleViewport, "Toogles maximize view", wxMEMORY_BITMAP(application_split), "Toogles maximize view" );
	toolbarCtrl->AddTool( Toolbar_ToogleSidebar, "Shows/hides the sidebar", wxMEMORY_BITMAP(application_side_tree_right), "Shows/hides the sidebar" );

	wxAuiPaneInfo toolPane;
	const char* name = "Toolbar";
	toolPane.ToolbarPane().Name(name).Caption(name).Top().PaneBorder(false);
	paneCtrl->AddPane(toolbarCtrl, toolPane);
}

//-----------------------------------//

void EditorFrame::redrawView()
{
	// Do this on the document instead.
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

	const DocumentsVector& documents = documentManager->getDocuments();

	for( size_t i = 0; i < documents.size(); i++ )
	{
		Document* document = documents[i];
		if( document->getWindow() == window ) return document;
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
		documentManager->currentDocument = nullptr;
	}

	if(newDocument)
	{
		documentManager->currentDocument = newDocument;
		newDocument->onDocumentSelect();
		eventManager->onDocumentSelect(*newDocument);
	}
}

//-----------------------------------//

void EditorFrame::onNotebookPageClose(wxAuiNotebookEvent& event)
{
	Document* document = getDocumentFromPage( event.GetSelection() );
	document->onDocumentUnselect();
	documentManager->removeDocument(document);
}

//-----------------------------------//

void EditorFrame::onDocumentAdded(Document* document)
{
	eventManager->onDocumentCreate(*document);

	getNotebook()->AddPage(document->getWindow(), document->getName());
	getAUI()->Update();
}

//-----------------------------------//

void EditorFrame::onDocumentRemoved(Document* document)
{
	eventManager->onDocumentDestroy(*document);

	wxWindow* window = document->getWindow();
	size_t index = notebookCtrl->GetPageIndex(window);
	notebookCtrl->RemovePage(index);
}

//-----------------------------------//

void EditorFrame::OnToolbarButtonClick(wxCommandEvent& event)
{
	const int id = event.GetId();

	switch(id) 
	{
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
#if 0
		if( !mainSplitter->GetWindow2() )
			mainSplitter->SplitVertically(viewSplitter, notebookCtrl, -220);
		else
			mainSplitter->Unsplit();

		break;
#endif
	} }
}

//-----------------------------------//

NAMESPACE_EDITOR_END