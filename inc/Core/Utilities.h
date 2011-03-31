/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include <sstream>

namespace vapor {

//---------------------------------------------------------------------//
// System Information
//---------------------------------------------------------------------//

namespace System
{
	// Returns if the system is little-endian.
	bool isLittleEndian();

	// Swaps the endianness of a long.
	long swapEndian(long i);
}

//---------------------------------------------------------------------//
// Locales
//---------------------------------------------------------------------//

struct LocaleSwitch
{
public:

	LocaleSwitch() { c = setlocale(LC_NUMERIC, "C"); }
	~LocaleSwitch() { setlocale(LC_NUMERIC, c); }

private:

	char* c;
};

//-----------------------------------//

} // end namespace