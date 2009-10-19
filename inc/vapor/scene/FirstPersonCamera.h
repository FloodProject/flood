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

namespace vapor {
	namespace scene {

//-----------------------------------//

/**
 * Implements a simple First-Person style camera.
 */

class VAPOR_API FirstPersonCamera : public Camera
{
public:

	FirstPersonCamera( input::InputManager* input,
		render::Device* device, 
		Projection::Enum projection = Projection::Perspective );

	// Sets the sensivity of the camera.
	void setSensivity( float sensivity );

	// Updates this node.
	virtual void update( double delta );

	// Serializes this node.
	virtual const std::string save( int indent = 0 );
	
	// Gets the name of this node.
	virtual const std::string name() const;

private:

	// Camera sensivity.
	float sensivity;
	float cameraSensivity;

	// Last mouse position.
	math::Vector3 lastPosition;

	// Input manager.
	input::InputManager* inputManager;

	// Register input devices callbacks.
	void registerCallbacks();

	// Centers the cursor position on the screen.
	void centerCursor();

	// Checks the controls for updates.
	void checkControls();

	// Input callback functions.
	void onKeyPressed( const input::KeyEvent& keyEvent );
	void onMouseMove( const input::MouseMoveEvent& moveEvent );
	void onButtonPressed( const input::MouseButtonEvent& buttonEvent );
	void onButtonReleased( const input::MouseButtonEvent& buttonEvent );
};

//-----------------------------------//

TYPEDEF_SHARED_POINTER_FROM_CLASS( FirstPersonCamera );

//-----------------------------------//

} } // end namespaces
