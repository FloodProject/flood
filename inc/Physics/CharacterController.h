/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "controllers/Controller.h"

#ifdef VAPOR_PHYSICS_BULLET

class btKinematicController;
class btPairCachingGhostObject;

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

REFLECT_DECLARE_CLASS(CharacterController)

class API_ENGINE CharacterController : public Controller
{
	REFLECT_DECLARE_OBJECT(CharacterController)

public:

	CharacterController();
	virtual ~CharacterController();

	// Updates the shape.
	void _update( float delta );

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

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( CharacterController );

//-----------------------------------//

NAMESPACE_ENGINE_END

#endif