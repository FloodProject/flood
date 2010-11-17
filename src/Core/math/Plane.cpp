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

Plane::Plane( const Vector3& _normal, const Vector3& point )
	: normal(_normal)
{
	normal.normalize();
	offset = -normal.dot(point);
}

//-----------------------------------//

Plane::Plane( const Vector3& _normal, float distance )
	: normal(_normal)
	, offset(distance)
{
	normal.normalize();
}

//-----------------------------------//

Vector3 Plane::project(const Vector3& vec)
{
	return vec - normal*vec.dot(normal);
}

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