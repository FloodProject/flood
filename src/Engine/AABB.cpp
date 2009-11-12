/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#include "vapor/math/AABB.h"

namespace vapor {
	namespace math {

const float f_inf = std::numeric_limits<float>::infinity();
const float f_min = std::numeric_limits<float>::min();
const float f_max = std::numeric_limits<float>::max();

//-----------------------------------//

AABB::AABB()
	: min( f_max ), max( f_min )
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
	if( aabb.min.x > max.x ) max.x = aabb.min.x;
	if( aabb.min.y < min.y ) min.y = aabb.min.y;
	if( aabb.min.y > max.y ) max.y = aabb.min.y;
	if( aabb.min.z < min.z ) min.z = aabb.min.z;
	if( aabb.min.z > max.z ) max.z = aabb.min.z;
}

//-----------------------------------//

} } // end namespaces