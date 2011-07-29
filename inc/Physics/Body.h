/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#ifdef VAPOR_PHYSICS_BULLET

#include "Scene/Component.h"

class btRigidBody;
class btMotionState;
class btCollisionShape;

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

class BodyMotionState;

REFLECT_DECLARE_CLASS(Body)

class API_ENGINE Body : public Component
{
	REFLECT_DECLARE_OBJECT(Body)

public:

	Body();
	virtual ~Body();

	// Returns if this body is dynamic.
	bool isDynamic() const;

	// Called once per frame to update the component.
	void update( float delta );

	// Gets the Bullet shape of this body.
	btCollisionShape* getBulletShape() const;

	// Updates the physics properties.
	void updateProperties();

	// Adds this body to the world.
	void addWorld();

	// Removes this body from the world.
	void removeWorld();

	// Gets the internal Bullet rigid body.
	GETTER(BulletBody, btRigidBody*, body)

protected:

	// Field change callback.
	void onFieldChanged(const Field& field);

	// Transform change callback.
	void onTransform();

	// Creates the body.
	bool createBody();

	// Rigid body mass.
	float mass;

	// Rigid body friction.
	float friction;

	// Rigid body restitution.
	float restitution;

	// Motion state of the body.
	BodyMotionState* motionState;

	// Internal rigid body.
	btRigidBody* body;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Body );

//-----------------------------------//

NAMESPACE_ENGINE_END

#endif