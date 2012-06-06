/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

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

// Generates a UUID (universally unique identifier).
//API_CORE int64 SystemGenerateUUID();

//---------------------------------------------------------------------//
// Filesystem helpers
//---------------------------------------------------------------------//

API_CORE bool FileExists(const Path&);
API_CORE void FileEnumerateFiles(const Path&, std::vector<Path>&);
API_CORE void FileEnumerateDirectories(const Path&, std::vector<Path>&);

//---------------------------------------------------------------------//
// Locales
//---------------------------------------------------------------------//

struct LocaleSwitch
{
	LocaleSwitch() { context = setlocale(LC_NUMERIC, "C"); }
	~LocaleSwitch() { setlocale(LC_NUMERIC, context); }
	char* context;
};

//-----------------------------------//

NAMESPACE_EXTERN_END