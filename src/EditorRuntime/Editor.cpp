/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Editor/API.h"
#include "Editor.h"
#include "EditorSettings.h"

#include "Core/PluginManager.h"
#include "Core/Utilities.h"

#include <wx/webview.h>
#include <wx/debugrpt.h>

FL_INSTANTIATE_TEMPLATES()

#ifdef BUILD_DEBUG
wxIMPLEMENT_WXWIN_MAIN_CONSOLE
#else
wxIMPLEMENT_WXWIN_MAIN
#endif

wxIMPLEMENT_APP_NO_MAIN(EditorApp);

int main(int argc, char **argv)
{
    wxDISABLE_DEBUG_SUPPORT();
    return wxEntry(argc, argv);
}

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

bool EditorApp::OnInit()
{
	if( !wxApp::OnInit() ) return false;

	wxImage::AddHandler( new wxPNGHandler() );

	mainFrame = new EditorFrame(VAPOR_EDITOR_NAME);
	mainFrame->SetSize(900, 550);
	mainFrame->SetIcon( wxIcon("iconEditor") );

	SetTopWindow(mainFrame);
	mainFrame->Show(true);

	return true;
}

//-----------------------------------//

static EditorFrame* gs_EditorInstance = nullptr;;
EditorFrame& GetEditor() { return *gs_EditorInstance; }

EditorFrame::EditorFrame(const wxString& title)
	: wxFrame(nullptr, wxID_ANY, title)
	, paneCtrl(nullptr)
	, toolbarCtrl(nullptr)
	, statusCtrl(nullptr)
	, notebookCtrl(nullptr)
	, engine(nullptr)
	, archive(nullptr)
	, input(nullptr)
{
	gs_EditorInstance = this;

	CoreInitialize();

#ifdef EDITOR_OLD_UI
	createUI();
#endif

	createEngine();

#ifdef ENABLE_PLUGIN_MONO
	Plugin* monoPlugin = pluginManager->getPluginFromClass( ReflectionGetType(MonoPlugin) );
	pluginManager->enablePlugin(monoPlugin);
#endif

	Bind(wxEVT_IDLE, &EditorFrame::OnIdle, this);
	Bind(wxEVT_CLOSE_WINDOW, &EditorFrame::OnClose, this);
}

//-----------------------------------//

EditorFrame::~EditorFrame()
{
	ArchiveDestroy(archive);

	Deallocate(input);

#ifdef EDITOR_OLD_UI
	if(notebookCtrl)
	{
		notebookCtrl->Destroy();
		paneCtrl->DetachPane(notebookCtrl);
	}

	if(paneCtrl)
	{
		paneCtrl->UnInit();
		delete paneCtrl;
	}
#endif

	Deallocate(engine);

	CoreDeinitialize();
}

//-----------------------------------//

void EditorFrame::OnIdle(wxIdleEvent& event)
{
	GetEngine()->update();
}

//-----------------------------------//

void EditorFrame::OnClose(wxCloseEvent& event)
{
	// Hide the window in advance so the ugly destroy is not seen.
	Hide();

	// Skip the event so the window frame is destroyed.
	event.Skip();
}

//-----------------------------------//

void EditorFrame::createEngine()
{
	engine = AllocateThis(Engine);
	engine->init();

	// Setup the input manager.
	input = AllocateThis(InputManager);
	input->createDefaultDevices();
	engine->setInputManager(input);

	// Mount the default assets path.
	ResourceManager* res = engine->getResourceManager();
	
	// Get the mount paths from the editor preferences.
	archive = ArchiveCreateVirtual( GetResourcesAllocator() );
	ArchiveMountDirectories(archive, MediaFolder, GetResourcesAllocator());
	
	res->setArchive(archive);
}

//-----------------------------------//

void EditorFrame::createUI()
{
	paneCtrl = new wxAuiManager();
	paneCtrl->SetManagedWindow(this);

	// Create notebook
	notebookCtrl = new wxAuiNotebook(this);
	
	wxAuiPaneInfo pane;
	pane.CenterPane().PaneBorder(false);
	paneCtrl->AddPane(notebookCtrl, pane);
}

//-----------------------------------//

NAMESPACE_EDITOR_END