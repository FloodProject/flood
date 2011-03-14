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

#include "Platform.h"

//---------------------------------------------------------------------//
// API Exports
//---------------------------------------------------------------------//

#if defined(CORE_API_DLL) && defined(CORE_API_DLL_EXPORT)
	#ifdef VAPOR_PLATFORM_WINDOWS
		#define CORE_API __declspec(dllexport)
	#else
		#define CORE_API __attribute__ ((visibility("default")))
	#endif
#elif defined(CORE_API_DLL) && defined(VAPOR_PLATFORM_WINDOWS)
	#define CORE_API __declspec(dllimport)
#else
	#define CORE_API
#endif

#define BEGIN_NAMESPACE_EXTERN namespace vapor { extern "C" {
#define END_NAMESPACE_EXTERN } }

//---------------------------------------------------------------------//
// External Templates
//---------------------------------------------------------------------//

#ifndef SWIG

#ifdef INSTANTIATE_TEMPLATES
template class std::basic_string<char>;
#else
extern template class std::basic_string<char>;
#endif

#endif

#include "Core/String.h"
