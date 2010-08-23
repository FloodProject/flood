/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/scene/Component.h"

class btRigidBody;
class btMotionState;
class btCollisionShape;

namespace vapor {

//-----------------------------------//

class BodyMotionState;

//-----------------------------------//

class VAPOR_API Body : public Component
{
	DECLARE_CLASS_()

public:

	Body();
	virtual ~Body();

	// Called once per frame to update the component.
	void update( double delta );

	// Gets the Bullet shape of this body.
	btCollisionShape* getBulletShape() const;

	// Gets the internal Bullet rigid body.
	GETTER(BulletBody, btRigidBody*, body)

protected:

	// Field change callback.
	void onFieldChanged(const Field& field);

	// Transform change callback.
	void onTransform();

	// Creates the body.
	bool createBody();
	
	// Adds this body to the world.
	void addWorld();

	// Removes this body from the world.
	void removeWorld();

	// Rigid body mass.
	float mass;

	// Motion state of the body.
	BodyMotionState* motionState;

	// Internal rigid body.
	btRigidBody* body;
};

TYPEDEF_SHARED_POINTER_FROM_TYPE( Body );
TYPEDEF_SHARED_WEAK_POINTER_FROM_TYPE( Body );

//-----------------------------------//

} // end namespace
