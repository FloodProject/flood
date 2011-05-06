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

namespace vapor {

//-----------------------------------//

class Window;

class InputManager;

struct KeyEvent;
struct MouseMoveEvent;
struct MouseDragEvent;
struct MouseWheelEvent;

//-----------------------------------//

/**
 * A simple First-Person style camera controller.
 */

REFLECT_DECLARE_CLASS(FirstPersonController)

class VAPOR_API FirstPersonController : public CameraController
{
	REFLECT_DECLARE_OBJECT(FirstPersonController)

public:

	FirstPersonController();
	~FirstPersonController();

	// Updates this component.
	virtual void _update( float delta );

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
	void onWindowFocusChange( bool focusLost );

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

	// Focus state.
	bool hasFocus;

	Window* window;
	InputManager* inputManager;
};

TYPEDEF_SHARED_POINTER_FROM_TYPE( FirstPersonController );

//-----------------------------------//

} // end namespace
