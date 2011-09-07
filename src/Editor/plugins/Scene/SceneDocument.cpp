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
#include "EventManager.h"
#include "Settings.h"
#include "RenderControl.h"
#include "EditorIcons.h"
#include "ResourceDrop.h"
#include "Core/Utilities.h"
#include "EditorPlugin.h"
#include "Core/PluginManager.h"
#include "Plugins/Selection/SelectionPlugin.h"
#include "Physics/Physics.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

SceneDocument::SceneDocument()
	: viewframe(nullptr)
	, toolbar(nullptr)
{
	createView();
	reset();

	ResourceManager* res = GetResourceManager();
	res->loadQueuedResources();
}

//-----------------------------------//

SceneDocument::~SceneDocument()
{
	LogDebug("Destroying SceneDocument");

	viewframe->getControl()->stopFrameLoop();
	viewframe->setMainCamera(nullptr);

	resetUndo();
	resetScene();

	viewframe->destroyControl();
	viewframe->Destroy();
}

//-----------------------------------//

static const char* s_FileDialogDescription( "Scene files (*.scene)|*.scene" );

bool SceneDocument::open()
{
	// Ask for file name to open.
	wxFileDialog fc( &GetEditor(), wxFileSelectorPromptStr, wxEmptyString,
		wxEmptyString, s_FileDialogDescription, wxFC_OPEN );
	
	if( fc.ShowModal() != wxID_OK )
		return true;

	Path path = (String) fc.GetPath();

	Serializer* serializer = SerializerCreateJSON( AllocatorGetThis() );
	Scene* object = (Scene*) SerializerLoadObjectFromFile(serializer, path);
	Deallocate(serializer);

	if( !object ) return false;

	Scene* newScene = (Scene*) object;
	setScene(newScene);

	return true;
}

//-----------------------------------//

bool SceneDocument::save()
{
	// Ask for file name to save as.
	wxFileDialog fc( &GetEditor(), wxFileSelectorPromptStr, wxEmptyString,
		wxEmptyString, s_FileDialogDescription, wxFC_SAVE | wxFD_OVERWRITE_PROMPT );
	
	if( fc.ShowModal() != wxID_OK )
		return true;

	Path path = (String) fc.GetPath();
	
	Serializer* serializer = SerializerCreateJSON( AllocatorGetThis() );
	bool res = SerializerSaveObjectToFile(serializer, path, scene.get());
	Deallocate(serializer);

	return res;
}

//-----------------------------------//

bool SceneDocument::reset()
{
	resetUndo();
	createUndo();

	setScene(nullptr);

	return true;
}

//-----------------------------------//

void SceneDocument::setScene( Scene* newScene )
{
	resetScene();

	if( !newScene )
		newScene = AllocateThis(Scene);

	scene.reset(newScene);
	createEditorScene();

	EventManager* events = GetEditor().getEventManager();
	events->onSceneLoad(scene);

	// Update the scenes.
	onUpdate(0);
}

//-----------------------------------//

void SceneDocument::resetScene()
{
	if(scene)
	{
		EventManager* events = GetEditor().getEventManager();
		events->onSceneUnload(scene);
	}

	cameraController.reset();

#if 0
	if( editorScene )
		assert( editorScene.get()->references == 1 );

	if( scene )
		assert( scene.get()->references == 1 );
#endif

	editorScene.reset();
	scene.reset();
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

void SceneDocument::createView()
{
	viewframe = new Viewframe( &GetEditor() );

	toolbar = createContextToolbar();
	toolbar->Realize();

	RenderControl* control = viewframe->createControl();
	control->onRender.Bind( this, &SceneDocument::onRender );
	control->onUpdate.Bind( this, &SceneDocument::onUpdate );
	control->Bind(wxEVT_RIGHT_UP, &SceneDocument::OnMouseRightUp, this); 
	control->Bind(wxEVT_RIGHT_DOWN, &SceneDocument::OnMouseRightDown, this);
	control->Bind(wxEVT_LEFT_DOWN, &SceneDocument::OnMouseEvent, this);
	control->Bind(wxEVT_RIGHT_DOWN, &SceneDocument::OnMouseEvent, this);
	control->Bind(wxEVT_MOUSEWHEEL, &SceneDocument::OnMouseEvent, this);
	control->SetDropTarget( new ResourceDropTarget( &GetEditor() ) );
	control->SetFocus();
	setupRenderWindow();

	RenderView* view = viewframe->createView();
	view->setClearColor(SceneEditClearColor);

	viewframe->mainSizer->Add(toolbar, wxSizerFlags().Expand().Top());
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

void SceneDocument::createEditorScene()
{
	Allocator* alloc = AllocatorGetHeap();

	editorScene = Allocate(Scene, alloc);
	
	// Create a grid entity.
	GridPtr grid = Allocate(Grid, alloc);
	grid->update(0);
	grid->updateBounds();

	EntityPtr entityGrid = EntityCreate(alloc);
	entityGrid->setName("Grid");
	entityGrid->addTransform();
	entityGrid->addComponent(grid);
	entityGrid->setTag( Tags::NonPickable, true );
	editorScene->add( entityGrid );

#ifdef ENABLE_PHYSICS_BULLET
	BoxShapePtr shape = Allocate(BoxShape, alloc);
	entityGrid->addComponent(shape);

	BodyPtr body = Allocate(Body, alloc);
	body->setMass(0);

	entityGrid->addComponent(body);
#endif

	// Create the camera entity.
	Vector3 initialPosition(0, 20, -65);
	EntityPtr entityCamera = createCamera();
	entityCamera->getTransform()->setPosition(initialPosition);
	editorScene->add( entityCamera );

	CameraPtr camera = entityCamera->getComponent<Camera>();
	viewframe->setMainCamera(camera);
	viewframe->switchToDefaultCamera();

#ifdef ENABLE_PHYSICS_BULLET
	DeallocateObject(GetEngine()->getPhysicsManager());
	PhysicsManager* physics = AllocateThis(PhysicsManager);
	physics->createWorld();
	GetEngine()->setPhysicsManager(physics);
#endif
}

//-----------------------------------//

EntityPtr SceneDocument::createCamera()
{
	Allocator* alloc = AllocatorGetHeap();

	// So each camera will have unique names.
	static uint8 i = 0;

	// Create a new first-person camera for our view.
	// By default it will be in perspective projection.
	CameraPtr camera = Allocate(Camera, alloc);
	cameraController = Allocate(FirstPersonController, alloc);
	cameraController->setEnabled(false);

	Frustum& frustum = camera->getFrustum();
	frustum.farPlane = 10000;

	// Generate a new unique name.
	String name( "EditorCamera"+StringFromNumber(i++) );

	EntityPtr entityCamera = EntityCreate(alloc);
	entityCamera->setName(name);
	entityCamera->addTransform();
	entityCamera->addComponent( camera );
	entityCamera->addComponent( cameraController );

#ifdef ENABLE_AUDIO_OPENAL
	ComponentPtr listener = Allocate(Listener, alloc);
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

	for( size_t i = 0; i < camera->drawer.renderables.size(); i++)
		block.renderables.push_back( camera->drawer.renderables[i] );

	camera->render(block);

	GetEngine()->stepFrame();

#ifdef ENABLE_PHYSICS_DEBUG
	PhysicsManager* physics = GetEngine()->getPhysicsManager();
	physics->drawDebugWorld();
#endif
}

//-----------------------------------//

void SceneDocument::onUpdate( float delta )
{
	if( !scene || !editorScene ) return;

	scene->update(delta);
	editorScene->update( delta );
	
#ifdef ENABLE_PHYSICS_BULLET
	//physicsManager->update( delta );
#endif

	//eventManager->onSceneUpdate();

	if(getRenderWindow()->isCursorVisible() && cameraController->getEnabled())
	{
		LogDebug("Camera failsafe, this should not happen");
		cameraController->setEnabled(false);
	}
}

//-----------------------------------//

NAMESPACE_EDITOR_END