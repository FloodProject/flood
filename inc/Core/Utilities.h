/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Core/String.h"
#include <type_traits>

NAMESPACE_CORE_BEGIN

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
API_CORE void FileEnumerateFiles(const Path&, Array<Path>&);
API_CORE void FileEnumerateDirectories(const Path&, Array<Path>&);

//---------------------------------------------------------------------//
// Locales
//---------------------------------------------------------------------//

struct LocaleSwitch
{
	LocaleSwitch() { context = setlocale(LC_NUMERIC, "C"); }
	~LocaleSwitch() { setlocale(LC_NUMERIC, context); }
	char* context;
};

//---------------------------------------------------------------------//
// Flags
//---------------------------------------------------------------------//

template <typename T>
inline bool GetBitFlag(T flags, T flag, typename std::enable_if<!std::is_enum<T>::value>::type* = 0)
{
    return ((flags & flag) ? true : false);
}

template <typename E>
inline bool GetBitFlag(E flags, E flag, typename std::enable_if<std::is_enum<E>::value>::type* = 0)
{
    typedef std::underlying_type<E>::type ET;
    return (((ET)flags & (ET)flag) ? true : false);
}

template <typename T>
inline void SetBitFlag(T& flags, T flag, bool state, typename std::enable_if<!std::is_enum<T>::value>::type* = 0)
{
    flags = (T)((state) ? (flags | flag) : (flags & ~flag));
}

template <typename E>
inline void SetBitFlag(E& flags, E flag, bool state, typename std::enable_if<std::is_enum<E>::value>::type* = 0)
{
    typedef std::underlying_type<E>::type ET;
    flags = (E)(((ET)state) ? ((ET)flags | (ET)flag) : ((ET)flags & ~(ET)flag));
}

//-----------------------------------//

NAMESPACE_CORE_END