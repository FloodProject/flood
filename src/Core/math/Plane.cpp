/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core.h"
#include "math/Plane.h"
#include "math/Math.h"

namespace vapor {

//-----------------------------------//

Plane::Plane()
{ }

//-----------------------------------//

Plane::Plane( const Vector3& p1, const Vector3& p2, const Vector3& p3 )
{ }

//-----------------------------------//

Plane::Plane( const Vector3& normal, float distance )
	: normal(normal)
	, offset(distance)
{ }

//-----------------------------------//

bool Plane::intersects( const Ray& ray, float& distance ) const
{
	// Gets the angle between the ray and the plane normal.
	float angle = ray.direction.dot(normal);

	// If they are perpendicular, then they will never intersect.
	if( Math::floatEqual(angle, 0) )
		return false;

	float delta = offset - ray.origin.dot(normal);
	distance = delta / angle;

	return true;
}

//-----------------------------------//

void Plane::normalize()
{
	float magnitude = normal.length();
	assert( magnitude > 0.0f );

	// Normalize the normal.
	normal.x /= magnitude;
	normal.y /= magnitude;
	normal.z /= magnitude;

	// We also need to divide the offset.
	offset /= magnitude;
}

//-----------------------------------//

} // end namespace