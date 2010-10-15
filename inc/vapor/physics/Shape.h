/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/scene/Component.h"

class btCollisionShape;

namespace vapor {

//-----------------------------------//

class Body;
TYPEDEF_SHARED_POINTER_FROM_TYPE(Body)
TYPEDEF_SHARED_WEAK_POINTER_FROM_TYPE(Body)

//-----------------------------------//

class VAPOR_API Shape : public Component
{
	DECLARE_CLASS_()

public:

	virtual ~Shape();

	// Gets the internal Bullet shape.
	virtual btCollisionShape* const getBulletShape() const = 0;

	// Sets the associated body.
	SETTER(Body, const BodyPtr&, weakBody)

protected:

	// Removes the body from the world.
	virtual void removeBody();

	// Weak pointer to body.
	BodyWeakPtr weakBody;
};

TYPEDEF_SHARED_POINTER_FROM_TYPE( Shape );
TYPEDEF_SHARED_WEAK_POINTER_FROM_TYPE( Shape );

//-----------------------------------//

} // end namespace
