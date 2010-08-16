/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/scene/Camera.h"
#include "vapor/input/InputManager.h"
#include "vapor/input/Keyboard.h"
#include "vapor/input/Mouse.h"
#include "vapor/math/Vector2.h"

namespace vapor {

//-----------------------------------//

/**
 * Implements a simple First-Person style camera.
 */

class VAPOR_API FirstPersonCamera : public Camera
{
	//DECLARE_CLASS_()

public:

	FirstPersonCamera( RenderDevice* device );

	// Updates this component.
	virtual void update( double delta );

	// Gets/sets the look sensivity of the camera.
	ACESSOR(LookSensivity, float, lookSensivity);
	
	// Gets/sets the move sensivity of the camera.
	ACESSOR(MoveSensivity, float, moveSensivity);

protected:

	// Register input devices callbacks.
	void registerCallbacks();

	// Centers the cursor position on the screen.
	void centerCursor();

	// Checks the controls for updates.
	void checkControls( double delta );

	// Input callback functions.
	void onKeyPressed( const KeyEvent& event );
	void onKeyReleased( const KeyEvent& event );
	void onMouseMove( const MouseMoveEvent& event );
	void onMouseDrag( const MouseDragEvent& event );
	void onMouseWheel( const MouseWheelEvent& event );
	void onWindowFocusChange( bool focusLost );

	// Relative movement position.
	Vector3 relativePosition;

	// Restrict camera movement on the X-axis.
	bool clampMovementX;

	// Camera sensivity.
	float lookSensivity;
	float moveSensivity;

	// Mouse movement.
	Vector2i oldMousePosition;
	Vector2i lastPosition;
	Vector2i mouseDistance;
	int mouseWheel;

	// Focus state.
	bool hasFocus;

	InputManager* inputManager;
};

TYPEDEF_SHARED_POINTER_FROM_TYPE( FirstPersonCamera );

//-----------------------------------//

} // end namespace
