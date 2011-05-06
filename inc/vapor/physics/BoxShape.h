/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#ifdef VAPOR_PHYSICS_BULLET

#include "Physics/Shape.h"

class btBoxShape;

namespace vapor {

//-----------------------------------//

REFLECT_DECLARE_CLASS(BoxShape)

class VAPOR_API BoxShape : public Shape
{
	REFLECT_DECLARE_OBJECT(BoxShape)

public:

	BoxShape();
	virtual ~BoxShape();

	// Updates the shape.
	void update( float delta );

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