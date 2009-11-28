/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/scene/Camera.h"

#include "vapor/input/InputManager.h"
#include "vapor/input/Keyboard.h"
#include "vapor/input/Mouse.h"

#include "vapor/math/Vector3.h"

namespace vapor { namespace scene {

//-----------------------------------//

/**
 * Implements a simple First-Person style camera. Needs some logic fixing.
 */

class VAPOR_API FirstPersonCamera : public Camera
{
public:

	FirstPersonCamera( input::InputManager* input,
		render::Device* device, 
		Projection::Enum projection = Projection::Perspective );

	// Sets the sensivity of the camera.
	void setLookSensivity( float sensivity );
	void setMoveSensivity( float sensivity );

	// Updates this node.
	virtual void update( float delta );

	// Serializes this node.
	virtual const std::string save( int indent = 0 );
	
	// Gets the name of this node.
	virtual const std::string name() const;

protected:

	// Register input devices callbacks.
	void registerCallbacks();

	// Centers the cursor position on the screen.
	void centerCursor();

	// Checks the controls for updates.
	void checkControls( double delta );

	// Input callback functions.
	void onKeyPressed( const input::KeyEvent& event );
	void onKeyReleased( const input::KeyEvent& event );
	void onMouseMove( const input::MouseMoveEvent& event );
	void onMouseWheel( const input::MouseWheelEvent& event );
	void onButtonPressed( const input::MouseButtonEvent& event );
	void onButtonReleased( const input::MouseButtonEvent& event );

	// Restrict camera movement on the X-axis.
	bool clampMovementX;

	// Camera sensivity.
	float lookSensivity;
	float moveSensivity;

	// Last mouse position (last frame).
	math::Vector3 lastPosition;

	// Holds the mouse distance travelled in a frame.
	math::Vector3 mouseDistance;

	// Input manager.
	input::InputManager* inputManager;

	//math::Vector3 rotateVectorKey;
};

//-----------------------------------//

TYPEDEF_SHARED_POINTER_FROM_CLASS( FirstPersonCamera );

//-----------------------------------//

} } // end namespaces
