/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "DocumentWindow.h"

FWD_DECL(RenderView)

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

class RenderControl;

/**
 * SceneWindow displays a view of a camera.
 */

class SceneWindow : public DocumentWindow
{
public:

	SceneWindow( wxWindow* parent );
	virtual ~SceneWindow();

	// Creates and adds a control to the SceneWindow.
	RenderControl* createControl();

	// Destroys the current control in the SceneWindow.
	void destroyControl();

	// Gets the associated control.
	GETTER(Control, RenderControl*, control)

	// Creates and adds a new view to the SceneWindow.
	RenderView* createView();

	// Gets the associated view.
	GETTER(View, RenderView*, view)

	// Flags the backing control for redraw.
	void flagRedraw() OVERRIDE;

	// Switches to the default camera.
	void switchToDefaultCamera();

	// Gets the main camera of the SceneWindow.
	GETTER(MainCamera, const CameraWeakPtr&, mainCamera)

	// Sets the main camera of the SceneWindow.
	SETTER(MainCamera, const CameraPtr&, mainCamera)

	// Gets called when camera changes in the view.
	void onCameraChanged(const CameraPtr& camera);

	// Camera of the SceneWindow.
	CameraWeakPtr mainCamera;

	// Control where the scene will be drawn.
	RenderControl* control;

	// Engine view associated with the control.
	RenderView* view;
};

typedef SceneWindow Viewframe;

//-----------------------------------//

NAMESPACE_EDITOR_END