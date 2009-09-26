/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"

namespace vapor {

//-----------------------------------//

/**
 * Small utility methods to swap endianness of data.
 */

namespace Endianness
{
	// Swaps the endianness of a long.
	long swap(long i);
}

//-----------------------------------//

} // end namespace
