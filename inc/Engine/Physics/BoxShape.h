/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#ifdef ENABLE_PHYSICS_BULLET

#include "Physics/Shape.h"

class btBoxShape;

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

REFLECT_DECLARE_CLASS(BoxShape)

class API_ENGINE BoxShape : public Shape
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

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( BoxShape );

//-----------------------------------//

NAMESPACE_ENGINE_END

#endif