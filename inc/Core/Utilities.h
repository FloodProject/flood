/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include <sstream>

NAMESPACE_EXTERN_BEGIN

//---------------------------------------------------------------------//
// System Information
//---------------------------------------------------------------------//

// Returns if the system is little-endian.
API_CORE bool SystemIsLittleEndian();

// Swaps the endianness of a long.
API_CORE int32 SystemSwapEndian(int32 i);

// Sleeps the current thread for the specified time.
API_CORE void SystemSleep( int64 time );

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

NAMESPACE_EXTERN_END