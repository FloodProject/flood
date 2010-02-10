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

