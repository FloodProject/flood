/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

namespace vapor {
	namespace math {

//-----------------------------------//

/**
 * Simple struct to hold dimensions.
 */

template< typename T >
struct DimensionT
{
public:

	DimensionT( T x, T y ) : width(x), height(y) {}
	
	// Width.
	T width;

	// Height.
	T height;
};

//-----------------------------------//

typedef DimensionT< int > Dimension;

//-----------------------------------//

} } // end namespaces

