/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/controllers/Controller.h"

class btKinematicController;
class btPairCachingGhostObject;

namespace vapor {

//-----------------------------------//

class VAPOR_API CharacterController : public Controller
{
	DECLARE_CLASS_()

public:

	CharacterController();
	virtual ~CharacterController();

	// Updates the shape.
	void _update( double delta );

protected:

	// Field change callback.
	void onFieldChanged(const Field& field);

	// Creates the character controller.
	bool createController();

	// Updates the properties of the controller.
	void updateProperties();

	// Updates the transform of the Bullet controller.
	void updateBulletTransform();

	// Updates the transform of the character.
	void updateCharacterTransform();

	// Stepping height.
	float stepHeight;

	// Walking velocity.
	float walkVelocity;
	
	// Falling speed.
	float fallSpeed;

	// Jumping speed.
	float jumpSpeed;

	// Maximum jump height.
	float maxJumpHeight;

	// Maximum climb slope.
	float maxSlope;

	// Bullet character controller.
	btKinematicController* controller;

	// Bullet ghost collision object.
	btPairCachingGhostObject* ghostObject;
};

TYPEDEF_SHARED_POINTER_FROM_TYPE( CharacterController );
TYPEDEF_SHARED_WEAK_POINTER_FROM_TYPE( CharacterController );

//-----------------------------------//

} // end namespace
