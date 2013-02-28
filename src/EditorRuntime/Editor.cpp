/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Editor/API.h"
#include "Editor.h"
#include "EditorSettings.h"

#include "SceneWindow.h"
#include "RenderControl.h"
#include "RenderWindow.h"

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

	mainFrame = new EditorFrame(EDITOR_NAME);
	mainFrame->SetSize(900, 550);
	mainFrame->SetIcon( wxIcon("EditorIcon") );

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
	, engine(nullptr)
	, archive(nullptr)
	, input(nullptr)
{
	gs_EditorInstance = this;

	CoreInitialize();

	createEngine();
	createUI();

#ifdef ENABLE_PLUGIN_MONO
	Plugin* monoPlugin = pluginManager->getPluginFromClass<MonoPlugin>();
	pluginManager->enablePlugin(monoPlugin);
#endif

	Bind(wxEVT_IDLE, &EditorFrame::OnIdle, this);
	Bind(wxEVT_CLOSE_WINDOW, &EditorFrame::OnClose, this);
}

//-----------------------------------//

EditorFrame::~EditorFrame()
{
	sceneWindow->getControl()->stopFrameLoop();
	Deallocate(sceneWindow);

	ArchiveDestroy(archive);

	Deallocate(input);
	Deallocate(engine);

	CoreDeinitialize();
}

//-----------------------------------//

void EditorFrame::OnIdle(wxIdleEvent& event)
{
	GetEngine()->update();
}

//-----------------------------------//

void CloseGUI();

void EditorFrame::OnClose(wxCloseEvent& event)
{
	CloseGUI();

	// Hide the window in advance so the ugly destroy is not seen.
	Hide();

	// Skip the event so the window frame is destroyed.
	event.Skip();
}

//-----------------------------------//

void InitializeGUI(InputManager*);

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

	InitializeGUI(input);
}

//-----------------------------------//

void EditorFrame::createUI()
{
	sceneWindow = new SceneWindow(this);
	sceneWindow->createControl();
	sceneWindow->createView();

	RenderControl* control = sceneWindow->getControl();
	control->onUpdate.Bind(this, &EditorFrame::onUpdate);
	control->onRender.Bind(this, &EditorFrame::onRender);
	control->startFrameLoop();
	control->SetFocus();

	Window* window = control->getRenderWindow();
	window->onTargetResize.Connect(this, &EditorFrame::onResize);

	RenderDevice* device = GetRenderDevice();
	device->setRenderTarget(window);
	window->getContext()->init();

	Camera* camera = AllocateHeap(Camera);

	Frustum& frustum = camera->getFrustum();
	frustum.farPlane = 10000;

	sceneWindow->setMainCamera(camera);
	sceneWindow->switchToDefaultCamera();
}

//-----------------------------------//

void RenderGUI(RenderBlock& rb);

void EditorFrame::onRender()
{
	Camera* camera = sceneWindow->getCamera().get();
	assert( camera != nullptr );

	RenderView* view = sceneWindow->getView();
	camera->setView(view);

	RenderBlock block;

	RenderGUI(block);
	camera->render(block);

	GetEngine()->stepFrame();
}

//-----------------------------------//

void EditorFrame::onUpdate(float delta)
{

}

//-----------------------------------//

void ResizeGUI(int x, int y);

void EditorFrame::onResize(const Settings& settings)
{
	ResizeGUI(settings.width, settings.height);
}

//-----------------------------------//

NAMESPACE_EDITOR_END