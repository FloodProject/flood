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
 * Simple struct to hold positions.
 */

template< typename T >
struct PointT
{
public:

	PointT( T x, T y ) : x(x), y(y) {}
	
	// X-coordinate of the point.
	T x;

	// Y-coordinate of the point.
	T y;
};

//-----------------------------------//

typedef PointT< int > Point;

//-----------------------------------//

} } // end namespaces

