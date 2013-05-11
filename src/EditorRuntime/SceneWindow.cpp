/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Editor/API.h"
#include "SceneWindow.h"
#include "RenderWindow.h"
#include "RenderControl.h"
#include "Engine/Scene/Entity.h"
#include "Engine/Scene/Transform.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

SceneWindow::SceneWindow( wxWindow* parent ) : wxPanel(
	parent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
	wxFULL_REPAINT_ON_RESIZE | wxBORDER_NONE)
{
	sizer = new wxBoxSizer( wxVERTICAL );
	SetSizer(sizer);
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
	sizer->Add(control, 1, wxEXPAND);

	return control;
}

//-----------------------------------//

void SceneWindow::switchToDefaultCamera()
{
	Camera* camera = mainCamera.get();
	if( !camera ) return;

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
