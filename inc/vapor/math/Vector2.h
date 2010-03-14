/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

namespace vapor { namespace math {

//-----------------------------------//

/**
 * Simple struct to hold 2-dimensional stuff.
 */

template< typename T >
struct Vector2T
{
public:
	
	Vector2T() {}
	Vector2T( T x, T y ) : x(x), y(y) {}
	
	// X-coordinate of the point.
	T x;

	// Y-coordinate of the point.
	T y;
};

//-----------------------------------//

typedef Vector2T<int> Vector2;
typedef Vector2T<int> Vector2i;

//-----------------------------------//

} } // end namespaces

