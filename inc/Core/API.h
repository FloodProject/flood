/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

//---------------------------------------------------------------------//
// Configuration
//---------------------------------------------------------------------//

#include "CompileOptions.h"

//---------------------------------------------------------------------//
// Platform headers
//---------------------------------------------------------------------//

#include "Core/Platform.h"

//---------------------------------------------------------------------//
// API Exports
//---------------------------------------------------------------------//

#if defined(API_CORE_DLL) && defined(API_CORE_DLL_EXPORT)
	#ifdef VAPOR_PLATFORM_WINDOWS
		#define API_CORE __declspec(dllexport)
	#else
		#define API_CORE __attribute__ ((visibility("default")))
	#endif
#elif defined(API_CORE_DLL) && defined(VAPOR_PLATFORM_WINDOWS)
	#define API_CORE __declspec(dllimport)
#else
	#define API_CORE
#endif

#define NAMESPACE_BEGIN namespace vapor {
#define NAMESPACE_END }

#ifdef __cplusplus
#define EXTERN_BEGIN extern "C" {
#define EXTERN_END }
#else
#define EXTERN_BEGIN
#define EXTERN_END
#endif

#define NAMESPACE_EXTERN_BEGIN NAMESPACE_BEGIN EXTERN_BEGIN
#define NAMESPACE_EXTERN_END EXTERN_END NAMESPACE_END

//---------------------------------------------------------------------//
// String
//---------------------------------------------------------------------//

#include "Core/String.h"

#ifndef SWIG

#ifdef VAPOR_COMPILER_MSVC
#pragma warning(disable : 4231)
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

namespace vapor { struct MemoryAllocator; }