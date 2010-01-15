/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/Endianness.h"

namespace vapor {

long Endianness::swap(long i)
{
	unsigned char b1, b2, b3, b4;

	b1 = i & 255;
	b2 = ( i>>8 )  & 255;
	b3 = ( i>>16 ) & 255;
	b4 = ( i>>24 ) & 255;

	return ((int)b1 << 24) + ((int)b2 << 16) + ((int)b3 << 8) + b4;
}

} // end namespace
