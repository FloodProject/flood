/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/math/BoundingBox.h"
#include "vapor/math/Math.h"

namespace vapor {

//-----------------------------------//

AABB::AABB()
	: min( Limits::FloatMaximum )
	, max( Limits::FloatMinimum )
{ }

//-----------------------------------//

AABB::AABB( const Vector3& min, const Vector3& max )
	: min( min )
	, max( max )
{ }

//-----------------------------------//

AABB::AABB( const AABB& box )
	: min( box.min )
	, max( box.max )
{ }

//-----------------------------------//

Vector3 AABB::getCorner( int i ) const
{
	assert(i >= 0 && i <= 7);

	return Vector3(
		(i & 1) ? max.x : min.x,
		(i & 2) ? max.y : min.y,
		(i & 4) ? max.z : min.z );
}

//-----------------------------------//

Vector3 AABB::getCenter() const
{
	return (min + max) * 0.5;
}

//-----------------------------------//

void AABB::reset()
{
	min = Limits::FloatMaximum;
	max = Limits::FloatMinimum;
}

//-----------------------------------//

void AABB::add( const Vector3& v )
{
	min.x = std::min(min.x, v.x);
	min.y = std::min(min.y, v.y);
	min.z = std::min(min.z, v.z);

	max.x = std::max(max.x, v.x);
	max.y = std::max(max.y, v.y);
	max.z = std::max(max.z, v.z);

	//if( v.x < min.x ) min.x = v.x;
	//if( v.x > max.x ) max.x = v.x;
	//if( v.y < min.y ) min.y = v.y;
	//if( v.y > max.y ) max.y = v.y;
	//if( v.z < min.z ) min.z = v.z;
	//if( v.z > max.z ) max.z = v.z;
}

//-----------------------------------//

void AABB::add( const AABB& aabb )
{
	if( aabb.min.x < min.x ) min.x = aabb.min.x;
	if( aabb.max.x > max.x ) max.x = aabb.max.x;
	if( aabb.min.y < min.y ) min.y = aabb.min.y;
	if( aabb.max.y > max.y ) max.y = aabb.max.y;
	if( aabb.min.z < min.z ) min.z = aabb.min.z;
	if( aabb.max.z > max.z ) max.z = aabb.max.z;
}

//-----------------------------------//

AABB AABB::transform( const Matrix4x3& mat ) const
{
	AABB box;
	
	for(int i = 0; i < 8; i++)
	{
		Vector3 pt = getCorner(i)*mat;
		box.add(pt);
	}
	
	return box;
}

//-----------------------------------//

// From "Fast Ray-Box Intersection," by Woo in Graphics Gems I,
// page 395.

bool AABB::intersects( const Ray& ray, float& distance ) const
{
	float lowt = 0.0f;
	float t;

	bool hit = false;
	Vector3 hitpoint;

	// Check origin inside first
	if ( ray.origin > min && ray.origin < max )
	{
		distance = 0;
		return true;
	}

	// Check each face in turn, only check closest 3
	// Min x
	if (ray.origin.x <= min.x && ray.direction.x > 0)
	{
		t = (min.x - ray.origin.x) / ray.direction.x;
		if (t >= 0)
		{
			// Substitute t back into ray and check bounds and dist
			hitpoint = ray.origin + ray.direction * t;
			if (hitpoint.y >= min.y && hitpoint.y <= max.y &&
				hitpoint.z >= min.z && hitpoint.z <= max.z &&
				(!hit || t < lowt))
			{
				hit = true;
				lowt = t;
			}
		}
	}
	// Max x
	if (ray.origin.x >= max.x && ray.direction.x < 0)
	{
		t = (max.x - ray.origin.x) / ray.direction.x;
		if (t >= 0)
		{
			// Substitute t back into ray and check bounds and dist
			hitpoint = ray.origin + ray.direction * t;
			if (hitpoint.y >= min.y && hitpoint.y <= max.y &&
				hitpoint.z >= min.z && hitpoint.z <= max.z &&
				(!hit || t < lowt))
			{
				hit = true;
				lowt = t;
			}
		}
	}
	// Min y
	if (ray.origin.y <= min.y && ray.direction.y > 0)
	{
		t = (min.y - ray.origin.y) / ray.direction.y;
		if (t >= 0)
		{
			// Substitute t back into ray and check bounds and dist
			hitpoint = ray.origin + ray.direction * t;
			if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
				hitpoint.z >= min.z && hitpoint.z <= max.z &&
				(!hit || t < lowt))
			{
				hit = true;
				lowt = t;
			}
		}
	}
	// Max y
	if (ray.origin.y >= max.y && ray.direction.y < 0)
	{
		t = (max.y - ray.origin.y) / ray.direction.y;
		if (t >= 0)
		{
			// Substitute t back into ray and check bounds and dist
			hitpoint = ray.origin + ray.direction * t;
			if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
				hitpoint.z >= min.z && hitpoint.z <= max.z &&
				(!hit || t < lowt))
			{
				hit = true;
				lowt = t;
			}
		}
	}
	// Min z
	if (ray.origin.z <= min.z && ray.direction.z > 0)
	{
		t = (min.z - ray.origin.z) / ray.direction.z;
		if (t >= 0)
		{
			// Substitute t back into ray and check bounds and dist
			hitpoint = ray.origin + ray.direction * t;
			if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
				hitpoint.y >= min.y && hitpoint.y <= max.y &&
				(!hit || t < lowt))
			{
				hit = true;
				lowt = t;
			}
		}
	}
	// Max z
	if (ray.origin.z >= max.z && ray.direction.z < 0)
	{
		t = (max.z - ray.origin.z) / ray.direction.z;
		if (t >= 0)
		{
			// Substitute t back into ray and check bounds and dist
			hitpoint = ray.origin + ray.direction * t;
			if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
				hitpoint.y >= min.y && hitpoint.y <= max.y &&
				(!hit || t < lowt))
			{
				hit = true;
				lowt = t;
			}
		}
	}

	distance = lowt;
	return hit;
}

//-----------------------------------//

} // end namespace