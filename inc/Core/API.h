/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "../Build/Build.h"
#include "Core/Platform.h"

#ifdef PLATFORM_NACL
#undef ENABLE_NETWORK_ENET
#undef ENABLE_NETWORKING_CURL
#undef ENABLE_HTTP_SERVER
#endif

//---------------------------------------------------------------------//
// Namespaces
//---------------------------------------------------------------------//

#define FLD_NAMESPACE_BEGIN
#define FLD_NAMESPACE_END

#ifdef NAMESPACE_CORE_DEFINED
#define NAMESPACE_CORE flood
#else
#define NAMESPACE_CORE
#endif

#ifdef NAMESPACE_CORE_DEFINED
#define NAMESPACE_CORE_BEGIN namespace NAMESPACE_CORE {
#define NAMESPACE_CORE_END }
#else
#define NAMESPACE_CORE_BEGIN
#define NAMESPACE_CORE_END
#endif

//---------------------------------------------------------------------//
// API Exports
//---------------------------------------------------------------------//

#if defined(API_CORE_DLL) && defined(API_CORE_DLL_EXPORT)
	#define API_CORE API_EXPORT
#elif defined(API_CORE_DLL)
	#define API_CORE API_IMPORT
#else
	#define API_CORE
#endif

#ifndef SWIG

#ifdef COMPILER_MSVC
// nonstandard extension used: 'identifier' before template explicit instantiation
#pragma warning(disable : 4231)

// nonstandard extension used: enum 'identifier' used in qualified name
#pragma warning(disable : 4482)
#endif

#define FL_INSTANTIATE_TEMPLATES() \
    template class std::basic_string<char>;

//extern template class std::basic_string<char>;

#endif

//---------------------------------------------------------------------//
// Forward declarations
//---------------------------------------------------------------------//

NAMESPACE_CORE_BEGIN

API_CORE void CoreInitialize();
API_CORE void CoreDeinitialize();

struct Allocator;

NAMESPACE_CORE_END