/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "../Build/Build.h"
#include "Core/Platform.h"

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

#undef NAMESPACE_CORE_DEFINED

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

#if defined(__cplusplus) || defined(SWIG)
#define EXTERN_BEGIN extern "C" {
#define EXTERN_END }
#else
#define EXTERN_BEGIN
#define EXTERN_END
#endif

#define NAMESPACE_EXTERN_BEGIN NAMESPACE_CORE_BEGIN EXTERN_BEGIN
#define NAMESPACE_EXTERN_END EXTERN_END NAMESPACE_CORE_END

//---------------------------------------------------------------------//
// String
//---------------------------------------------------------------------//

#include "Core/String.h"

#ifndef SWIG

#ifdef COMPILER_MSVC
// nonstandard extension used: 'identifier' before template explicit instantiation
#pragma warning(disable : 4231)

// nonstandard extension used: enum 'identifier' used in qualified name
#pragma warning(disable : 4482)
#endif

#define FL_INSTANTIATE_TEMPLATES() \
    template class std::basic_string<char>;

extern template class std::basic_string<char>;

#endif

//---------------------------------------------------------------------//
// Forward declarations
//---------------------------------------------------------------------//

NAMESPACE_CORE_BEGIN

API_CORE void CoreInitialize();
API_CORE void CoreDeinitialize();

struct Allocator;

NAMESPACE_CORE_END