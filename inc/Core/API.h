/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

//---------------------------------------------------------------------//
// Core APIs
//---------------------------------------------------------------------//

//#define   ENABLE_STL_RDE
//#define   ENABLE_MEMORY_TR1_VENDOR
//#define   ENABLE_MEMORY_SHARED_PTR
#define     ENABLE_MEMORY_INTRUSIVE_PTR
#define     ENABLE_MEMORY_LEAK_DETECTOR
//#define   ENABLE_DYNAMIC_LIBRARY
#define     ENABLE_ARCHIVE_ZIP
#define     ENABLE_ARCHIVE_DIR
#define     ENABLE_ARCHIVE_VIRTUAL
#define     ENABLE_NETWORKING_CURL
//#define   ENABLE_VFS_FILEWATCHER
#define     ENABLE_SERIALIZATION
#define     ENABLE_SERIALIZATION_JSON
#define     ENABLE_SERIALIZATION_BINARY
#define     ENABLE_STACK_WALKER
#define     ENABLE_HTTP_SERVER

//---------------------------------------------------------------------//
// Platform headers
//---------------------------------------------------------------------//

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

#undef  NAMESPACE_CORE_DEFINED

#ifdef NAMESPACE_CORE_DEFINED
#define NAMESPACE_CORE vapor
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

#define NAMESPACE_BEGIN NAMESPACE_CORE_BEGIN
#define NAMESPACE_END NAMESPACE_CORE_END

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

#ifdef INSTANTIATE_TEMPLATES
template class std::basic_string<char>;
#else
extern template class std::basic_string<char>;
#endif

#endif

//---------------------------------------------------------------------//
// Forward declarations
//---------------------------------------------------------------------//

NAMESPACE_CORE_BEGIN

struct Allocator;

NAMESPACE_CORE_END