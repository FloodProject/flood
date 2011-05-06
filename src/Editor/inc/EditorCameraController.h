/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Controllers/CameraController.h"
#include "Math/Vector.h"
#include "Math/EulerAngles.h"
#include "Input/MouseEvents.h"

namespace vapor {

//-----------------------------------//

REFLECT_DECLARE_CLASS(EditorCameraController)

class VAPOR_API EditorCameraController : public CameraController
{
	REFLECT_DECLARE_OBJECT(EditorCameraController)

public:

	EditorCameraController();
	~EditorCameraController();

	// Updates this component.
	virtual void _update( float delta );

protected:

	void updateKeyboardMovement(float delta);
	void updateMouseMovement(float delta);
	void calculateView();

	// Input callback functions.
	void onMouseButtonPress( const MouseButtonEvent& event );
	void onMouseButtonRelease( const MouseButtonEvent& event );
	void onMouseDrag( const MouseDragEvent& event );
	void onMouseWheel( const MouseWheelEvent& event );

	int mouseWheel;
	Vector2i mouseDistance;
	Vector2i mousePosition;

	Vector3 relativePosition;
	EulerAngles accumulatedRotation;
	Vector3 moveVector;
};

TYPEDEF_SHARED_POINTER_FROM_TYPE( EditorCameraController );

//-----------------------------------//

} // end namespace
