/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/math/Ray.h"

namespace vapor { namespace math {

//-----------------------------------//

Ray::Ray( Vector3 origin, Vector3 direction ) 
	: origin( origin ), direction( direction )
{

}

//-----------------------------------//

Ray::Ray( const Ray& ray )
	: origin( ray.origin ), direction( ray.direction )
{

}

//-----------------------------------//

const Vector3& Ray::getOrigin() const
{
	return origin;
}

//-----------------------------------//

const Vector3& Ray::getDirection() const
{
	return direction;
}

//-----------------------------------//

bool Ray::intersects( const Vector3 tri[3], Vector3& intersectionPoint, float& t ) const 
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
	float u = tvec.dot(pvec) * inv_det;

	if (u < 0.0 || u > 1.0)
		return false;

	Vector3 qvec = tvec.cross(edge1);
	float v = direction.dot(qvec) * inv_det;

	if (v < 0.0 || (u + v) > 1.0)
		return false;

	t = edge2.dot(qvec) * inv_det;
	intersectionPoint = origin + (direction * t);

    return true;
}

//-----------------------------------//

} } // end namespaces