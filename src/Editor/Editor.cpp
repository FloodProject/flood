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

#include "DocumentManager.h"
#include "EventManager.h"

#include "Plugins/Project/ProjectPlugin.h"
#define CREATE_PROJECT_ON_STARTUP
//#define CREATE_WELCOME_SCREEN

#include <wx/webview.h>

wxIMPLEMENT_WXWIN_MAIN_CONSOLE
//wxIMPLEMENT_WXWIN_MAIN

wxIMPLEMENT_APP_NO_MAIN(EditorApp);

#include "Render/Program.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

bool EditorApp::OnInit()
{
	if( !wxApp::OnInit() ) return false;

	wxImage::AddHandler( new wxPNGHandler() );

	mainFrame = new EditorFrame(VAPOR_EDITOR_NAME);
	mainFrame->SetSize(800, 500);

	SetTopWindow(mainFrame);
	mainFrame->Show(true);

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
	, paneCtrl(nullptr)
	, toolbarCtrl(nullptr)
	, statusCtrl(nullptr)
	, notebookCtrl(nullptr)
	, eventManager(nullptr)
	, pluginManager(nullptr)
	, documentManager(nullptr)
	, engine(nullptr)
{
	gs_EditorInstance = this;

	createUI();
	createEngine();
	createPlugins();
	createToolbar();
	createLastUI();

	Bind(wxEVT_IDLE, &EditorFrame::OnIdle, this);
	Bind(wxEVT_CLOSE_WINDOW, &EditorFrame::OnClose, this);

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
	project->onNewDocument(event);
#endif

#ifdef CREATE_WELCOME_SCREEN
	// Create welcome screen.
	wxWebView* webView = wxWebView::New(this, wxID_ANY);
	webView->SetWindowStyle(wxBORDER_NONE);
	//webView->LoadURL("Layouts/Layout.html");

	notebookCtrl->AddPage(webView, "Welcome");
#endif

	getAUI()->Update();

	AllocatorDumpInfo();
}

//-----------------------------------//

EditorFrame::~EditorFrame()
{
	Deallocate(documentManager);

	eventManager->disconnectPluginListeners();

	pluginManager->disablePlugins();
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
		if( !plugin->isEnabled() ) continue;

		plugin->onPluginUpdate();
	}
}

//-----------------------------------//

void EditorFrame::OnClose(wxCloseEvent& event)
{
	#pragma TODO("Check for unsaved documents when closing");

	// Hide the window in advance so the ugly destroy is not seen.
	Hide();

	// Skip the event so the window frame is destroyed.
	event.Skip();
}

//-----------------------------------//

void EditorFrame::createPlugins()
{
	documentManager = AllocateThis(DocumentManager);
	documentManager->onDocumentAdded.Connect(this, &EditorFrame::onDocumentAdded);
	documentManager->onDocumentRemoved.Connect(this, &EditorFrame::onDocumentRemoved);
	documentManager->onDocumentRenamed.Connect(this, &EditorFrame::onDocumentRenamed);

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

class wxWideAuiToolbar : public wxAuiToolBar
{
public:

	wxWideAuiToolbar(wxWindow* parent, wxWindowID id = -1,
					const wxPoint& position = wxDefaultPosition,
					const wxSize& size = wxDefaultSize,
					long style = wxAUI_TB_DEFAULT_STYLE)
		: wxAuiToolBar(parent, id, position, size, style)
	{
		//Bind(wxEVT_SIZE, &wxWideAuiToolbar::OnSize, this);
	}

	void OnSize(wxSizeEvent& event)
	{
		wxAuiToolBar::OnSize(event);
		wxSize size = m_sizer->GetSize();
		m_sizer->SetDimension(0, 0, wxDefaultCoord, size.y);
	}
};

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
	toolbarCtrl = new wxWideAuiToolbar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, style);
	toolbarCtrl->SetGripperVisible(false);
	//toolbarCtrl->GetArtProvider()->SetElementSize(wxAUI_TBART_GRIPPER_SIZE, 0);
	toolbarCtrl->Bind(wxEVT_COMMAND_MENU_SELECTED, &EditorFrame::OnToolbarButtonClick, this);

#if 0
	// Create status-bar.
	statusCtrl = CreateStatusBar();
#endif
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

Document* EditorFrame::getDocumentFromPage(int selection)
{
	if( selection < 0 ) return nullptr;
	if( (size_t)selection >= notebookCtrl->GetPageCount() ) return nullptr;

	wxWindow* window = notebookCtrl->GetPage(selection);

	const DocumentsVector& documents = documentManager->getDocuments();

	for( size_t i = 0; i < documents.size(); i++ )
	{
		Document* document = documents[i];
		wxWindow* documentWindow = (wxWindow*) document->getWindow();

		if( documentWindow == window ) return document;
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
	if( !document ) return;

	documentManager->removeDocument(document);

	// We veto the default wxWidgets closed event because we
	// already delete the notebook page when a document is removed.

	event.Veto();
}

//-----------------------------------//

Document* EditorFrame::getDocument() const
{
	if( !documentManager ) return nullptr;
	return documentManager->currentDocument;
}

//-----------------------------------//

void EditorFrame::onDocumentAdded(Document* document)
{
	if( !document ) return;

	eventManager->onDocumentCreate(*document);

	wxWindow* window = (wxWindow*) document->getWindow();

	if( !window )
	{
		LogDebug("Invalid window in document");
		return;
	}

	String name = PathGetFile( document->getPath() );
	if( name.empty() ) name = "untitled";
	
	getNotebook()->AddPage(window, name, true, document->getBitmap());

	getAUI()->Update();
}

//-----------------------------------//

void EditorFrame::onDocumentRemoved(Document* document)
{
	if( !document ) return;

	eventManager->onDocumentDestroy(*document);
	
	wxWindow* window = (wxWindow*) document->getWindow();
	int index = notebookCtrl->GetPageIndex(window);

	if( index != wxID_INVALID )
		notebookCtrl->DeletePage(index);
}

//-----------------------------------//

void EditorFrame::onDocumentRenamed(Document* document)
{
	if( !document ) return;

	String name = PathGetFile( document->getPath() );
	if( name.empty() ) name = "untitled";

	if( document->getUnsavedChanges() )
		name += "*";

	wxWindow* window = (wxWindow*) document->getWindow();
	size_t index = notebookCtrl->GetPageIndex(window);

	notebookCtrl->SetPageText(index, name);
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