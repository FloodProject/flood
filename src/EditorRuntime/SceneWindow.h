/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Editor/API.h"
#include "Editor/RenderControl.h"
#include "Engine/Scene/Camera.h"

FWD_DECL(RenderView)

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

class RenderControl;

/**
 * SceneWindow displays a view of a camera.
 */

class SceneWindow : public wxPanel
{
public:

	SceneWindow( wxWindow* parent );
	virtual ~SceneWindow();

	// Creates and adds a control to the window.
	RenderControl* createControl();

	// Destroys the current control in the window.
	void destroyControl();

	// Gets the associated control.
	GETTER(Control, RenderControl*, control)

	// Creates and adds a new view to the window.
	RenderView* createView();

	// Gets the associated view.
	GETTER(View, RenderView*, view)

	// Flags the backing control for redraw.
	void flagRedraw();

	// Switches to the default camera.
	void switchToDefaultCamera();

	// Gets/sets the camera of the window.
	ACCESSOR(Camera, const CameraPtr&, camera)

	// Gets the main camera of the window.
	GETTER(MainCamera, const CameraPtr&, mainCamera)

	// Sets the main camera of the window.
	SETTER(MainCamera, const CameraPtr&, mainCamera)

	// Gets called when camera changes in the view.
	void onCameraChanged(const CameraPtr& camera);

	// Current camera of the window.
	CameraPtr camera;

	// Camera of the window.
	CameraPtr mainCamera;

	// Control where the scene will be drawn.
	RenderControl* control;

	// Engine view associated with the control.
	RenderView* view;

	wxBoxSizer* sizer;
};

//-----------------------------------//

NAMESPACE_EDITOR_END