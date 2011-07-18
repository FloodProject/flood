/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"
#include "Math/BoundingSphere.h"
#include "Core/Log.h"

NAMESPACE_BEGIN

//-----------------------------------//

BoundingSphere::BoundingSphere()
	: radius( 0 )
{ }

//-----------------------------------//

BoundingSphere::BoundingSphere( const Vector3& center, float radius )
	: center( center )
	, radius( radius )
{ }

//-----------------------------------//

BoundingSphere::BoundingSphere( const BoundingSphere& bs )
	: center( bs.center )
	, radius( bs.radius )
{ }

//-----------------------------------//

BoundingSphere::BoundingSphere( const BoundingBox& box )
	: center( box.getCenter() )
	, radius( (box.getCenter()-box.max.x).length() )
{ }

//-----------------------------------//

bool BoundingSphere::intersects( const Ray& ray, float& distance ) const
{
	assert( 0 );
	return false;
}

//-----------------------------------//

bool BoundingSphere::intersects( const BoundingSphere& sphere ) const
{
	Vector3 dist = center - sphere.center;
	float minDist = radius + sphere.radius;

	return dist.lengthSquared() <= minDist * minDist;
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

NAMESPACE_END