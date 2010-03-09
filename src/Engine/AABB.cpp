/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/math/AABB.h"

namespace vapor { namespace math {

static const float f_inf = std::numeric_limits<float>::infinity();
static const float f_min = std::numeric_limits<float>::min();
static const float f_max = std::numeric_limits<float>::max();

//-----------------------------------//

AABB::AABB()
	: min( f_max ), max( f_min )
{

}

//-----------------------------------//

AABB::AABB( const Vector3& min, const Vector3& max )
	: min( min ), max( max )
{

}

//-----------------------------------//

AABB::AABB( const AABB& box )
	: min( box.min ), max( box.max )
{

}

//-----------------------------------//

Vector3 AABB::getCorner( int i ) const
{
	assert(i >= 0);
	assert(i <= 7);

	return Vector3(
		(i & 1) ? max.x : min.x,
		(i & 2) ? max.y : min.y,
		(i & 4) ? max.z : min.z );
}

//-----------------------------------//

Vector3 AABB::getCenter() const
{
	return Vector3( (max.x-min.x)/2,
		(max.y-min.y)/2, (max.z-min.z)/2 );
}

//-----------------------------------//

void AABB::reset()
{
	min.x = f_max;
	min.y = f_max;
	min.z = f_max;

	max.x = f_min;
	max.y = f_min;
	max.z = f_min;
}

//-----------------------------------//

const Vector3& AABB::getMinimum() const
{
	return min;
}

//-----------------------------------//

const Vector3& AABB::getMaximum() const
{
	return max;
}

//-----------------------------------//

void AABB::add( const Vector3& v )
{
	if( v.x < min.x ) min.x = v.x;
	if( v.x > max.x ) max.x = v.x;
	if( v.y < min.y ) min.y = v.y;
	if( v.y > max.y ) max.y = v.y;
	if( v.z < min.z ) min.z = v.z;
	if( v.z > max.z ) max.z = v.z;
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
	//Vector3 nmin( mat.tx, mat.ty, mat.tz );
	//Vector3 nmax( min );

	//for ( uint i = 0; i < 3; i++ )
	//{
	//	for ( uint j = 0; j < 3; j++ )
	//	{
	//		float av = mat.element(i, j) * min[j];
	//		float bv = mat.element(i, j) * max[j];

	//		if (av < bv)
	//		{
	//			nmin[i] += av;
	//			nmax[i] += bv;
	//		} else {
	//			nmin[i] += bv;
	//			nmax[i] += av;
	//		}
	//	}
	//}

	return  AABB( min*mat, max*mat );
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

	const Vector3& rayorig = ray.getOrigin();
	const Vector3& raydir = ray.getDirection();

	// Check origin inside first
	if ( rayorig > min && rayorig < max )
	{
		distance = 0;
		return true;
	}

	// Check each face in turn, only check closest 3
	// Min x
	if (rayorig.x <= min.x && raydir.x > 0)
	{
		t = (min.x - rayorig.x) / raydir.x;
		if (t >= 0)
		{
			// Substitute t back into ray and check bounds and dist
			hitpoint = rayorig + raydir * t;
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
	if (rayorig.x >= max.x && raydir.x < 0)
	{
		t = (max.x - rayorig.x) / raydir.x;
		if (t >= 0)
		{
			// Substitute t back into ray and check bounds and dist
			hitpoint = rayorig + raydir * t;
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
	if (rayorig.y <= min.y && raydir.y > 0)
	{
		t = (min.y - rayorig.y) / raydir.y;
		if (t >= 0)
		{
			// Substitute t back into ray and check bounds and dist
			hitpoint = rayorig + raydir * t;
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
	if (rayorig.y >= max.y && raydir.y < 0)
	{
		t = (max.y - rayorig.y) / raydir.y;
		if (t >= 0)
		{
			// Substitute t back into ray and check bounds and dist
			hitpoint = rayorig + raydir * t;
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
	if (rayorig.z <= min.z && raydir.z > 0)
	{
		t = (min.z - rayorig.z) / raydir.z;
		if (t >= 0)
		{
			// Substitute t back into ray and check bounds and dist
			hitpoint = rayorig + raydir * t;
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
	if (rayorig.z >= max.z && raydir.z < 0)
	{
		t = (max.z - rayorig.z) / raydir.z;
		if (t >= 0)
		{
			// Substitute t back into ray and check bounds and dist
			hitpoint = rayorig + raydir * t;
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

} } // end namespaces