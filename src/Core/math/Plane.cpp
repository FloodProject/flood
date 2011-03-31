/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"
#include "Math/Plane.h"
#include "Math/Helpers.h"
#include "Core/Log.h"

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

Plane& Plane::operator = (const Plane& plane)
{
	normal = plane.normal;
	offset = plane.offset;
	
	return *this;
}

//-----------------------------------//

float Plane::distance(const Vector3& pt) const
{
	return distance(pt.x, pt.y, pt.z);
}

//-----------------------------------//

float Plane::distance(float x, float y, float z) const
{
	return normal.x*x
		 + normal.y*y
		 + normal.z*z
		 + offset;
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
	if( MathFloatCompare(angle, 0) )
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