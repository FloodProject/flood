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

} } // end namespaces