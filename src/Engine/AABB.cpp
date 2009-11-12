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

const float inf = std::numeric_limits<float>::infinity();

//-----------------------------------//

AABB::AABB()
	: min( std::numeric_limits<float>::max() ), 
	max( std::numeric_limits<float>::min() )
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
	min.x = inf;
	min.y = inf;
	min.z = inf;

	max.x = inf;
	max.y = inf;
	max.z = inf;
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
	if( v.y < min.x ) min.y = v.y;
	if( v.y > max.x ) max.y = v.y;
	if( v.z < min.x ) min.z = v.z;
	if( v.z > max.x ) max.z = v.z;
}

//-----------------------------------//

void AABB::add( const AABB& aabb )
{
	if( aabb.min.x < min.x ) min.x = aabb.min.x;
	if( aabb.min.x > max.x ) max.x = aabb.min.x;
	if( aabb.min.y < min.x ) min.y = aabb.min.y;
	if( aabb.min.y > max.x ) max.y = aabb.min.y;
	if( aabb.min.z < min.x ) min.z = aabb.min.z;
	if( aabb.min.z > max.x ) max.z = aabb.min.z;
}

//-----------------------------------//

} } // end namespaces