/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"
#include "Plugins/Scene/SceneWindow.h"
#include "RenderWindow.h"
#include "RenderControl.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

SceneWindow::SceneWindow( wxWindow* parent ) : DocumentWindow(parent)
{
}

//-----------------------------------//

SceneWindow::~SceneWindow()
{
	LogDebug("Destroying SceneWindow");
	destroyControl();
}

//-----------------------------------//

void SceneWindow::destroyControl()
{
	if( !control ) return;

	control->stopFrameLoop();
	control->onRender.clear();
	control->onUpdate.clear();
	control->Destroy();

	control = nullptr;
}

//-----------------------------------//

RenderControl* SceneWindow::createControl()
{
	int attribList[] =
	{
		WX_GL_RGBA,
		WX_GL_DOUBLEBUFFER,
		WX_GL_DEPTH_SIZE, 16,
#ifdef ENABLE_VIEWFRAME_ANTIALIAS
		WX_GL_SAMPLE_BUFFERS, 1,
		WX_GL_SAMPLES, 4,
#endif
		0
	};

	control = new RenderControl(this, wxID_ANY, attribList);
	mainSizer->Add( control, 1, wxEXPAND );

	return control;
}

//-----------------------------------//

void SceneWindow::switchToDefaultCamera()
{
	Camera* camera = mainCamera.get();
	if( !camera ) return;

#if 0
	EntityPtr entityCamera = camera->getEntity();
	ControllerPtr controller = entityCamera->getComponentFromFamily<Controller>();
	if( controller ) controller->setEnabled(true);
#endif

	setCamera(camera);
}

//-----------------------------------//

RenderView* SceneWindow::createView()
{
	Window* window = control->getRenderWindow();

	view = window->createView();
	//view->onCameraChanged.Connect(this, &SceneWindow::onCameraChanged);
	
	return view;
}

//-----------------------------------//

void SceneWindow::onCameraChanged(const CameraPtr& camera)
{
	flagRedraw();

	// Subscribe to the camera transform events.
	Transform* transform = camera->getEntity()->getTransform().get();
	transform->onTransform.Connect( this, &SceneWindow::flagRedraw );
}

//-----------------------------------//

void SceneWindow::flagRedraw()
{
	if( !control ) return;
	control->flagRedraw();
}

//-----------------------------------//

NAMESPACE_EDITOR_END
