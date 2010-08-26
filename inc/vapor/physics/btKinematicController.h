/*
Bullet Continuous Collision Detection and Physics Library
Copyright (c) 2003-2008 Erwin Coumans  http://bulletphysics.com

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the
use of this software.

Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely,
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim
that you wrote the original software. If you use this software in a product,
an acknowledgment in the product documentation would be appreciated but is
not required.

2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.

3. This notice may not be removed or altered from any source distribution.
*/

#pragma once

#include "LinearMath/btVector3.h"
#include "BulletDynamics/Character/btCharacterControllerInterface.h"
#include "BulletCollision/BroadphaseCollision/btCollisionAlgorithm.h"

class btCollisionShape;
class btRigidBody;
class btCollisionWorld;
class btCollisionDispatcher;
class btPairCachingGhostObject;
class btConvexShape;

/**
 * This is an object that supports a sliding motion in a world. It uses a ghost
 * object and convex sweep test to test for upcoming collisions. This is combined
 * with discrete collision detection to recover from penetrations. Interaction 
 * between the controller and dynamic rigid bodies needs to be explicity
 * implemented by the user.
 */

class btKinematicController : public btCharacterControllerInterface
{
public:

	btKinematicController(btPairCachingGhostObject* ghostObject, 
		btConvexShape* convexShape,btScalar stepHeight, int upAxis = 1);
	
	void updateAction( btCollisionWorld* collisionWorld, btScalar deltaTime);
	
	void debugDraw(btIDebugDraw* debugDrawer);

	void setUpAxis (int axis);

	/// This should probably be called setPositionIncrementPerSimulatorStep.
	/// This is neither a direction nor a velocity, but the amount to
	///	increment the position each simulation iteration, regardless
	///	of dt.
	/// This call will reset any velocity set by setVelocityForTimeInterval().
	virtual void setWalkDirection(const btVector3& walkDirection);

	/// Caller provides a velocity with which the character should move for
	///	the given time period.  After the time period, velocity is reset
	///	to zero.
	/// This call will reset any walk direction set by setWalkDirection().
	/// Negative time intervals will result in no motion.
	virtual void setVelocityForTimeInterval(const btVector3& velocity,
				btScalar timeInterval);

	void reset ();
	void warp (const btVector3& origin);

	void preStep (  btCollisionWorld* collisionWorld);
	void playerStep ( btCollisionWorld* collisionWorld, btScalar dt);

	// Sets the fall speed of the controller.
	SETTER(FallSpeed, btScalar, m_fallSpeed)

	// Sets the jump speed of the controller.
	SETTER(JumpSpeed, btScalar, m_jumpSpeed)

	// Sets the max jump height of the controller.
	SETTER(MaxJumpHeight, btScalar, m_maxJumpHeight)
	
	// Gets/sets the gravity of the controller.
	ACESSOR(Gravity, btScalar, m_gravity)
	
	void setMaxSlope(btScalar slopeRadians);
	btScalar getMaxSlope() const;

	// Jumping functionality.
	bool canJump () const;
	void jump ();

	/// The max slope determines the maximum angle that the controller can walk up.
	/// The slope angle is measured in radians.
	GETTER(GhostObject, btPairCachingGhostObject*, m_ghostObject)
	
	SETTER(UseGhostSweepTest, bool, m_useGhostObjectSweepTest)

	bool onGround () const;

protected:

	static btVector3* getUpAxisDirections();

	btVector3 computeReflectionDirection (const btVector3& direction, const btVector3& normal);
	btVector3 parallelComponent (const btVector3& direction, const btVector3& normal);
 	btVector3 perpindicularComponent (const btVector3& direction, const btVector3& normal);

	bool recoverFromPenetration ( btCollisionWorld* collisionWorld);
	void stepUp (btCollisionWorld* collisionWorld);
	void updateTargetPositionBasedOnCollision (const btVector3& hit_normal, btScalar tangentMag = btScalar(0.0), btScalar normalMag = btScalar(1.0));
	void stepForwardAndStrafe (btCollisionWorld* collisionWorld, const btVector3& walkMove);
	void stepDown (btCollisionWorld* collisionWorld, btScalar dt);

	btScalar m_halfHeight;
	
	btPairCachingGhostObject* m_ghostObject;
	btConvexShape*	m_convexShape;
	
	btScalar m_verticalVelocity;
	btScalar m_verticalOffset;
	btScalar m_fallSpeed;
	btScalar m_jumpSpeed;
	btScalar m_maxJumpHeight;
	btScalar m_maxSlopeRadians; // Slope angle that is set (used for returning the exact value)
	btScalar m_maxSlopeCosine;  // Cosine equivalent of m_maxSlopeRadians (calculated once when set, for optimization)
	btScalar m_gravity;

	btScalar m_turnAngle;
	
	btScalar m_stepHeight;

	btScalar	m_addedMargin;//@todo: remove this and fix the code

	///this is the desired walk direction, set by the user
	btVector3	m_walkDirection;
	btVector3	m_normalizedDirection;

	//some internal variables
	btVector3 m_currentPosition;
	btScalar  m_currentStepOffset;
	btVector3 m_targetPosition;

	///keep track of the contact manifolds
	btManifoldArray	m_manifoldArray;

	bool m_touchingContact;
	btVector3 m_touchingNormal;

	bool  m_wasOnGround;
	bool  m_wasJumping;
	bool	m_useGhostObjectSweepTest;
	bool	m_useWalkDirection;
	btScalar	m_velocityTimeInterval;
	int m_upAxis;
};