/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
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