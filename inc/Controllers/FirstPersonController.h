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

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

class Window;

class InputManager;

struct KeyEvent;
struct MouseMoveEvent;
struct MouseDragEvent;
struct MouseWheelEvent;

/**
 * A simple First-Person style camera controller.
 */

REFLECT_DECLARE_CLASS(FirstPersonController)

class API_SCENE FirstPersonController : public CameraController
{
	REFLECT_DECLARE_OBJECT(FirstPersonController)

public:

	FirstPersonController();
	~FirstPersonController();

	// Sets if the controller is enabled.
	void setEnabled(bool enabled) OVERRIDE;

	// Updates this component.
	void _update( float delta ) OVERRIDE;

protected:

	// Register input devices callbacks.
	void registerCallbacks();

	// Centers the cursor position on the screen.
	void centerCursor();

	// Checks the controls for updates.
	void checkControls( float delta );

	// Input callback functions.
	void onKeyPressed( const KeyEvent& event );
	void onKeyReleased( const KeyEvent& event );
	void onMouseMove( const MouseMoveEvent& event );
	void onMouseDrag( const MouseDragEvent& event );
	void onMouseWheel( const MouseWheelEvent& event );

	// Relative movement position.
	Vector3 relativePosition;
	EulerAngles rotation;

	// Restrict camera movement on the X-axis.
	bool clampMovementX;

	// Mouse movement.
	Vector2i oldMousePosition;
	Vector2i lastPosition;
	Vector2i mouseDistance;
	int mouseWheel;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( FirstPersonController );

//-----------------------------------//

NAMESPACE_ENGINE_END
