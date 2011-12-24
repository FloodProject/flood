/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"
#include "SceneDocument.h"
#include "SceneCamera.h"
#include "Editor.h"
#include "EventManager.h"
#include "Settings.h"
#include "RenderControl.h"
#include "EditorIcons.h"
#include "ResourceDrop.h"
#include "Core/Utilities.h"
#include "EditorPlugin.h"
#include "Plugins/Selection/SelectionPlugin.h"
#include "Plugins/Gizmos/GizmoPlugin.h"
#include "Physics/Physics.h"

#if defined(PLATFORM_WINDOWS) && defined(ENABLE_MEMORY_LEAK_DETECTOR)
#include <vld.h>
#endif

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

SceneDocument::SceneDocument()
	: sceneWindow(nullptr)
	, toolbar(nullptr)
{
}

//-----------------------------------//

SceneDocument::~SceneDocument()
{

}

//-----------------------------------//

void SceneDocument::onDocumentDestroy()
{
	LogDebug("Destroying SceneDocument");

	if( sceneWindow )
	{
		sceneWindow->getControl()->stopFrameLoop();
		sceneWindow->setCamera(nullptr);
		sceneWindow->setMainCamera(nullptr);
	}

	resetUndo();
	resetScene();

	if( sceneWindow )
	{
		sceneWindow->destroyControl();
	}
}

//-----------------------------------//

bool SceneDocument::onDocumentOpen()
{
#if defined(PLATFORM_WINDOWS) && defined(ENABLE_MEMORY_LEAK_DETECTOR)
	// Workaround VLD bug with file dialogs
	VLDDisable();
#endif

	// Ask for file name to open.
	wxFileDialog fc( &GetEditor(), wxFileSelectorPromptStr, wxEmptyString,
		wxEmptyString, getFileDialogDescription(), wxFC_OPEN );
	
	if( fc.ShowModal() != wxID_OK )
		return true;

#if defined(PLATFORM_WINDOWS) && defined(ENABLE_MEMORY_LEAK_DETECTOR)
	VLDRestore();
#endif

	reset();

	Path filePath = (String) fc.GetPath();

	Serializer* serializer = SerializerCreateJSON( AllocatorGetThis() );
	Scene* object = (Scene*) SerializerLoadObjectFromFile(serializer, filePath);
	Deallocate(serializer);

	if( !object ) return false;

	Scene* newScene = (Scene*) object;
	setScene(newScene);
	setPath(filePath);

	return true;
}

//-----------------------------------//

bool SceneDocument::onDocumentSave()
{
	Serializer* serializer = SerializerCreateJSON( AllocatorGetThis() );
	bool res = SerializerSaveObjectToFile(serializer, path, scene.get());
	Deallocate(serializer);

	return res;
}

//-----------------------------------//

bool SceneDocument::onDocumentReset()
{
	setScene(nullptr);

	return true;
}

//-----------------------------------//

const char* SceneDocument::getFileDialogDescription()
{
	static const char* s_FileDialogDescription( "Scene files (*.scene)|*.scene" );
	return s_FileDialogDescription;
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
	getRenderControl()->stopFrameLoop();

	if(scene)
	{
		EventManager* events = GetEditor().getEventManager();
		events->onSceneUnload(scene);
	}

	cameraController.reset();

#ifdef BUILD_DEBUG
	if( editorScene && editorScene.get()->references != 1 )
		LogAssert("Scene should not have any references");

	if( scene && scene.get()->references != 1 )
		LogAssert("Scene should not have any references");
#endif

	sceneWindow->setCamera(nullptr);
	sceneWindow->setMainCamera(nullptr);

	editorScene.reset();
	scene.reset();

#if 0
	const RenderViewsVector& views = getRenderWindow()->getViews();

	for(size_t i = 0; i < views.size(); ++i)
	{
		RenderView* view = views[i];
		view->setCamera(nullptr);
	}
#endif

	getRenderControl()->startFrameLoop();
}

//-----------------------------------//

void SceneDocument::onDocumentSelect()
{
	setupRenderWindow();

	RenderControl* control = sceneWindow->getControl();
	control->startFrameLoop();

	wxAuiManager* aui = GetEditor().getAUI();
	aui->GetPane("Hierarchy").Show();
	aui->GetPane("Properties").Show();
	aui->Update();
}

//-----------------------------------//

void SceneDocument::onDocumentUnselect()
{
	RenderControl* control = sceneWindow->getControl();
	control->stopFrameLoop();

#if 0
	#pragma TODO("Don't use hardcoded pane names")

	wxAuiManager* aui = GetEditor().getAUI();
	aui->GetPane("Hierarchy").Hide();
	aui->GetPane("Properties").Hide();
	aui->Update();
#endif
}

//-----------------------------------//

void SceneDocument::OnMouseEvent(wxMouseEvent& event)
{
	getRenderControl()->SetFocus();
	event.Skip();
}

//-----------------------------------//

void SceneDocument::OnMouseRightDown(wxMouseEvent& event)
{
	if(cameraController) cameraController->setEnabled(true);
	getRenderWindow()->setCursorVisiblePriority(false, 100);

	// Toogle the camera tool.
	GizmoPlugin* gizmoPlugin = GetPlugin<GizmoPlugin>();
	
	ToolExtension* cameraTool = gizmoPlugin->findToolById(GizmoTool::Camera);
	if( !cameraTool ) return;

	GetEditor().getEventManager()->toggleTool(cameraTool);

	//event.Skip();
}

//-----------------------------------//

void SceneDocument::OnMouseRightUp(wxMouseEvent& event)
{
	if(cameraController) cameraController->setEnabled(false);
	getRenderWindow()->setCursorVisiblePriority(true, 100);

	// Restore the original tool.
	GetEditor().getEventManager()->toggleTool(nullptr);

	event.Skip();
}

//-----------------------------------//

DocumentWindow* SceneDocument::createDocumentWindow()
{
	sceneWindow = new SceneWindow( &GetEditor() );

	RenderControl* control = sceneWindow->createControl();
	control->onRender.Bind( this, &SceneDocument::onRender );
	control->onUpdate.Bind( this, &SceneDocument::onUpdate );
	control->Bind(wxEVT_RIGHT_UP, &SceneDocument::OnMouseRightUp, this); 
	control->Bind(wxEVT_RIGHT_DOWN, &SceneDocument::OnMouseRightDown, this);
	control->Bind(wxEVT_LEFT_DOWN, &SceneDocument::OnMouseEvent, this);
	control->Bind(wxEVT_RIGHT_DOWN, &SceneDocument::OnMouseEvent, this);
	control->Bind(wxEVT_MOUSEWHEEL, &SceneDocument::OnMouseEvent, this);
	control->SetDropTarget( new ResourceDropTarget( &GetEditor() ) );
	control->SetFocus();

	RenderView* view = sceneWindow->createView();
	view->setClearColor(SceneEditClearColor);

	return sceneWindow;
}

//-----------------------------------//

RenderControl* SceneDocument::getRenderControl()
{
	return sceneWindow->getControl();
}

//-----------------------------------//

RenderWindow* SceneDocument::getRenderWindow()
{
	return getRenderControl()->getRenderWindow();
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

void SceneDocument::onToolSelect(ToolExtension* mode)
{

}

//-----------------------------------//

void SceneDocument::createEditorScene()
{
	Allocator* alloc = AllocatorGetHeap();

	editorScene = Allocate(alloc, Scene);
	
	// Create a grid entity.
	Grid* grid = Allocate(alloc, Grid);
	grid->update(0);
	grid->updateBounds();

	Entity* entityGrid = EntityCreate(alloc);
	entityGrid->setName("Grid");
	entityGrid->addTransform();
	entityGrid->addComponent(grid);
	entityGrid->setTag( Tags::NonPickable, true );
	editorScene->entities.add( entityGrid );

#ifdef ENABLE_PHYSICS_BULLET
	BoxShape* shape = Allocate(alloc, BoxShape);
	entityGrid->addComponent(shape);

	Body* body = Allocate(alloc, Body);
	body->setMass(0);

	entityGrid->addComponent(body);
#endif

	// Create the camera entity.
	Vector3 initialPosition(0, 20, -65);
	EntityPtr entityCamera = createCamera();
	entityCamera->getTransform()->setPosition(initialPosition);
	editorScene->entities.add( entityCamera );

	Camera* camera = entityCamera->getComponent<Camera>().get();
	sceneWindow->setMainCamera(camera);
	sceneWindow->switchToDefaultCamera();

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
	// So each camera will have unique names.
	static uint8 i = 0;

	// Create a new first-person camera for our view.
	// By default it will be in perspective projection.
	Camera* camera = AllocateHeap(Camera);
	cameraController = AllocateHeap(FirstPersonController);
	cameraController->setEnabled(false);

	Frustum& frustum = camera->getFrustum();
	frustum.farPlane = 10000;

	// Generate a new unique name.
	String name = StringFormat("EditorCamera%d", i++);

	Entity* entityCamera = EntityCreate( AllocatorGetHeap() );
	entityCamera->setName(name);
	entityCamera->addTransform();
	entityCamera->addComponent( camera );
	entityCamera->addComponent( cameraController );

#ifdef ENABLE_AUDIO_OPENAL
	Component* listener = AllocateHeap(Listener);
	entityCamera->addComponent( listener );
#endif

	return entityCamera;
}

//-----------------------------------//

void SceneDocument::onRender()
{
	Camera* camera = sceneWindow->getCamera().get();
	assert( camera != nullptr );

	RenderView* view = sceneWindow->getView();
	camera->setView( view );

	RenderBlock block;
	camera->cull( block, &scene->entities );
	camera->cull( block, &editorScene->entities );

#if 0
	for( size_t i = 0; i < camera->drawer.renderables.size(); i++)
		block.renderables.push_back( camera->drawer.renderables[i] );
#endif

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