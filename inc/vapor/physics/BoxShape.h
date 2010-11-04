/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#ifdef VAPOR_PHYSICS_BULLET

#include "vapor/physics/Shape.h"

class btBoxShape;

namespace vapor {

//-----------------------------------//

class VAPOR_API BoxShape : public Shape
{
	DECLARE_CLASS_()

public:

	BoxShape();
	virtual ~BoxShape();

	// Updates the shape.
	void update( double delta );

	// Gets the internal Bullet shape.
	btCollisionShape* const getBulletShape() const;

protected:

	// Bullet box shape.
	btBoxShape* boxShape;
};

TYPEDEF_SHARED_POINTER_FROM_TYPE( BoxShape );
TYPEDEF_SHARED_WEAK_POINTER_FROM_TYPE( BoxShape );

//-----------------------------------//

} // end namespace

#endif