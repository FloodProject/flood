/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/math/BoundingSphere.h"

namespace vapor {

//-----------------------------------//

BoundingSphere::BoundingSphere()
	: radius( 0.0f )
{

}

//-----------------------------------//

BoundingSphere::BoundingSphere( const Vector3& center, float radius )
	: center( center ), radius( radius )
{

}

//-----------------------------------//

BoundingSphere::BoundingSphere( const BoundingSphere& bs )
	: center( bs.center ), radius( bs.radius )
{

}

//-----------------------------------//

const Vector3& BoundingSphere::getCenter() const
{
	return center;
}

//-----------------------------------//

float BoundingSphere::getRadius() const
{
	return radius;
}

//-----------------------------------//

bool BoundingSphere::intersects( const Ray& ray, float& distance ) const
{
	return false;
}

//-----------------------------------//

bool BoundingSphere::intersects( const Vector3& pt ) const
{
    Vector3 v = center - pt;
    
	if( v.lengthSquared() > radius*radius )
        return false;
    
	return true;
}

//-----------------------------------//

} // end namespace