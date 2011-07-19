/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"
#include "SceneDocument.h"
#include "Editor.h"
#include "Events.h"
#include "Settings.h"
#include "RenderControl.h"
#include "EditorIcons.h"
#include "ResourceDrop.h"
#include "Core/Utilities.h"
#include "EditorPlugin.h"
#include "Core/PluginManager.h"
#include "../Selection/SelectionPlugin.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

SceneDocument::SceneDocument()
	: viewframe(nullptr)
	, toolbar(nullptr)
{
	createView();
	createScene();

	RenderControl* control = viewframe->getControl();
	control->Bind(wxEVT_RIGHT_UP, &SceneDocument::OnMouseRightUp, this); 
	control->Bind(wxEVT_RIGHT_DOWN, &SceneDocument::OnMouseRightDown, this);
	control->Bind(wxEVT_LEFT_DOWN, &SceneDocument::OnMouseEvent, this);
	control->Bind(wxEVT_RIGHT_DOWN, &SceneDocument::OnMouseEvent, this);
	control->Bind(wxEVT_MOUSEWHEEL, &SceneDocument::OnMouseEvent, this);

	ResourceManager* res = GetResourceManager();
	res->loadQueuedResources();

	// Update at least once before rendering.
	onUpdate(0);

	Events* events = GetEditor().getEventManager();
	events->onSceneLoad(scene);
}

//-----------------------------------//

SceneDocument::~SceneDocument()
{
	LogDebug("Destroying SceneDocument");

	resetScene();

	RenderControl* control = viewframe->getControl();
	control->stopFrameLoop();
	control->onRender.clear();
	control->onUpdate.clear();

	viewframe->destroyControl();
}

//-----------------------------------//

void SceneDocument::resetScene()
{
	Events* events = GetEditor().getEventManager();
	events->onSceneUnload(scene);

	assert( ReferenceGetCount(editorScene.get()) == 1 );
	//assert( ReferenceGetCount(scene.get()) == 1 );

	editorScene.reset();
	scene.reset();
	cameraController.reset();
}

//-----------------------------------//

void SceneDocument::OnMouseRightUp(wxMouseEvent& event)
{
	cameraController->setEnabled(false);
	getRenderWindow()->setCursorVisiblePriority(true, 100);

	event.Skip();
}

//-----------------------------------//

void SceneDocument::OnMouseRightDown(wxMouseEvent& event)
{
	cameraController->setEnabled(true);
	getRenderWindow()->setCursorVisiblePriority(false, 100);

	//event.Skip();
}

//-----------------------------------//

wxWindow* SceneDocument::getWindow()
{
	return viewframe;
}

//-----------------------------------//

void SceneDocument::setupRenderWindow()
{
	Window* window = getRenderWindow();

	RenderDevice* device = GetRenderDevice();
	device->setRenderTarget( window );
	window->getContext()->init();
}

//-----------------------------------//

void SceneDocument::onDocumentSelect()
{
	setupRenderWindow();

	RenderControl* control = viewframe->getControl();
	control->startFrameLoop();

	wxAuiManager* aui = GetEditor().getAUI();
	aui->GetPane("Hierarchy").Show();
	aui->GetPane("Properties").Show();
	aui->Update();

	GetEditor().getEventManager()->onSceneLoad(scene);
}

//-----------------------------------//

void SceneDocument::onDocumentUnselect()
{
	RenderControl* control = viewframe->getControl();
	control->stopFrameLoop();

	#pragma TODO("Don't use hardcoded pane names")

	wxAuiManager* aui = GetEditor().getAUI();
	aui->GetPane("Hierarchy").Hide();
	aui->GetPane("Properties").Hide();
	aui->Update();
}

//-----------------------------------//

void SceneDocument::OnMouseEvent(wxMouseEvent& event)
{
	getRenderControl()->SetFocus();
	event.Skip();
}

//-----------------------------------//

void SceneDocument::createView()
{
	viewframe = new Viewframe( &GetEditor() );

	toolbar = createContextToolbar();
	toolbar->Realize();

	RenderControl* control = viewframe->createControl();
	control->onRender.Bind( this, &SceneDocument::onRender );
	control->onUpdate.Bind( this, &SceneDocument::onUpdate );
	control->SetDropTarget( new ResourceDropTarget( &GetEditor() ) );
	control->SetFocus();
	setupRenderWindow();

	RenderView* view = viewframe->createView();
	view->setClearColor(SceneEditClearColor);

	viewframe->mainSizer->Add(toolbar, wxSizerFlags().Expand().Top());
}

//-----------------------------------//

wxAuiToolBar* SceneDocument::createContextToolbar()
{
	wxAuiToolBar* tb = new wxAuiToolBar(viewframe);
	return tb;
}

//-----------------------------------//

void SceneDocument::onToolSelect(PluginTool* mode)
{
	wxSizer* sizer = viewframe->mainSizer;
	wxSizerItem* item = sizer->GetChildren().GetLast()->GetData();
	
	wxWindow* currentWindow = item->GetWindow();
	currentWindow->Hide();

	wxAuiToolBar* newToolbar = (mode->toolbar) ? mode->toolbar : toolbar;
	newToolbar->Show();
	
	item->AssignWindow(newToolbar);
	sizer->Layout();
}

//-----------------------------------//

void SceneDocument::createScene()
{
	scene.reset( new Scene() );
	editorScene.reset( new Scene() );
	
	// Create a nice grid for the editor.
	EntityPtr nodeGrid( EntityCreate(AllocatorGetHeap()) );
	nodeGrid->setName("Grid");
	nodeGrid->addTransform();
	nodeGrid->addComponent( GridPtr( new Grid() ) );
	nodeGrid->setTag( Tags::NonPickable, true );
	editorScene->add( nodeGrid );

	Vector3 initialPosition(0, 20, -65);
	EntityPtr nodeCamera( createCamera() );
	nodeCamera->getTransform()->setPosition(initialPosition);
	editorScene->add( nodeCamera );

	CameraPtr camera = nodeCamera->getComponent<Camera>();
	viewframe->setMainCamera(camera);
	viewframe->switchToDefaultCamera();
}

//-----------------------------------//

EntityPtr SceneDocument::createCamera()
{
	// So each camera will have unique names.
	static uint8 i = 0;

	// Create a new first-person camera for our view.
	// By default it will be in perspective projection.
	CameraPtr camera( new Camera() );
	cameraController.reset( new FirstPersonController() );
	cameraController->setEnabled(false);

	Frustum& frustum = camera->getFrustum();
	frustum.farPlane = 10000;

	// Generate a new unique name.
	String name( "EditorCamera"+StringFromNumber(i++) );

	EntityPtr entityCamera( EntityCreate(AllocatorGetHeap()) );
	entityCamera->setName(name);
	entityCamera->addTransform();
	entityCamera->addComponent( camera );
	entityCamera->addComponent( cameraController );

#ifdef ENABLE_AUDIO_OPENAL
	ComponentPtr listener( new Listener() );
	entityCamera->addComponent( listener );
#endif

	return entityCamera;
}

//-----------------------------------//

void SceneDocument::onRender()
{
	RenderView* view = viewframe->getView();

	if( !view->getCamera() )
		viewframe->switchToDefaultCamera();

	const CameraPtr& camera = view->getCamera();
	if( !camera ) return;
	
	camera->setView( view );

	RenderBlock block;
	camera->cull( block, scene );
	camera->cull( block, editorScene );
	camera->render(block);

	GetEngine()->stepFrame();

#ifdef ENABLE_PHYSICS_BULLET
	PhysicsManager* physics = engine->getPhysicsManager();
	physics->drawDebugWorld();
#endif
}

//-----------------------------------//

void SceneDocument::onUpdate( float delta )
{
	#pragma TODO("Engine specific updates should not be done per scene")

	GetEngine()->update( delta );
	
	if(editorScene)
		editorScene->update( delta );
	//eventManager->onSceneUpdate();

	if(getRenderWindow()->isCursorVisible() && cameraController->getEnabled())
	{
		LogDebug("Camera failsafe, this should not happen");
		cameraController->setEnabled(false);
	}
}

//-----------------------------------//

NAMESPACE_EDITOR_END