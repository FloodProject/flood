/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"
#include "math/Ray.h"
#include "math/Helpers.h"

namespace vapor {

//-----------------------------------//

Ray::Ray() {}

//-----------------------------------//

Ray::Ray( const Vector3& origin, const Vector3& direction ) 
	: origin( origin )
	, direction( direction )
{ }

//-----------------------------------//

Ray::Ray( const Ray& ray )
	: origin( ray.origin )
	, direction( ray.direction )
{ }

//-----------------------------------//

Vector3 Ray::getPoint( float distance ) const
{
	return origin + direction*distance;
}

//-----------------------------------//

#if 0

bool Ray::intersects( const Vector3 tri[3], float& t, float& u, float& v ) const 
{ 
	// This code is based of the published code by Tomas Möller.
	// http://www.cs.lth.se/home/Tomas_Akenine_Moller/raytri/
	// Also thanks to LittleCodingFox for sharing the code with me.

	Vector3 v1 = tri[0];
	Vector3 v2 = tri[1];
	Vector3 v3 = tri[2];

	Vector3 edge1 = v2-v1;
	Vector3 edge2 = v3-v1;
	Vector3 pvec = direction.cross(edge2);

	float det = edge1.dot(pvec);

	if (det > -FLT_EPSILON && det < FLT_EPSILON)
		return false;

	float inv_det = 1.0f / det;
	Vector3 tvec = origin-v1;
	u = tvec.dot(pvec) * inv_det;

	if (u < 0.0 || u > 1.0)
		return false;

	Vector3 qvec = tvec.cross(edge1);
	v = direction.dot(qvec) * inv_det;

	if (v < 0.0 || (u + v) > 1.0)
		return false;

	t = edge2.dot(qvec) * inv_det;

    return true;
}

#endif

//-----------------------------------//

#if 1

bool Ray::intersects( const Vector3 (&tri)[3], float& t, float& u, float& v ) const 
{ 
	// This code is based on the published code by Tomas Möller.
	// http://www.cs.lth.se/home/Tomas_Akenine_Moller/raytri/
	// Also thanks to LittleCodingFox for sharing it with me.

	const Vector3& v1 = tri[0];
	const Vector3& v2 = tri[1];
	const Vector3& v3 = tri[2];

	Vector3 qvec;

	/* find vectors for two edges sharing vert0 */
	Vector3 edge1 = v2-v1;
	Vector3 edge2 = v3-v1;

	/* begin calculating determinant - also used to calculate U parameter */
	Vector3 pvec = direction.cross(edge2);

	/* if determinant is near zero, ray lies in plane of triangle */
	float det = edge1.dot(pvec);

	if (det > Limits::FloatEpsilon)
	{
		/* calculate distance from vert0 to ray origin */
		Vector3 tvec = origin-v1;

		/* calculate U parameter and test bounds */
		u = tvec.dot(pvec);
		
		if (u < 0.0 || u > det)
			return false;

		/* prepare to test V parameter */
		qvec = tvec.cross(edge1);

		/* calculate V parameter and test bounds */
		v = direction.dot(qvec);	
	
		if (v < 0.0 || (u + v) > det)
			return false;
	}
	else if(det < -Limits::FloatEpsilon)
	{
		/* calculate distance from vert0 to ray origin */
		Vector3 tvec = origin-v1;

		/* calculate U parameter and test bounds */
		u = tvec.dot(pvec);
		
		if (u > 0.0 || u < det)
			return false;

		/* prepare to test V parameter */
		qvec = tvec.cross(edge1);

		/* calculate V parameter and test bounds */
		v = direction.dot(qvec);	
	
		if (v > 0.0 || (u + v) < det)
			return false;
	}
	else return false; /* ray is parallell to the plane of the triangle */

	float inv_det = 1.0f / det;
	
	/* calculate t, ray intersects triangle */
	t = edge2.dot(qvec) * inv_det;
	u *= inv_det;
	v *= inv_det;
	
	return true;
}

#endif

//-----------------------------------//

} // end namespace