/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

//---------------------------------------------------------------------//
// Platform detection
//---------------------------------------------------------------------//

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
	#define PLATFORM_WINDOWS
#elif defined(__APPLE__) || defined(MACOSX)
	#define PLATFORM_MACOSX
#elif defined(__LINUX) || defined(__LINUX__) || defined(LINUX)
	#define PLATFORM_LINUX
#elif defined(__native_client__) || defined(__pnacl__)
	#define PLATFORM_NACL
#else
	#pragma warning Unknown platform
#endif

//---------------------------------------------------------------------//
// Compiler detection
//---------------------------------------------------------------------//

#ifdef __clang__
	#define COMPILER_CLANG
	#define COMPILER_SUPPORTS_CXX11
#elif defined(_MSC_VER)
	#if _MSC_VER == 1700
		#define COMPILER_MSVC_2012 _MSC_VER
		#define COMPILER_MSVC	COMPILER_MSVC_2012
	#elif _MSC_VER == 1600
		#define COMPILER_MSVC_2010 _MSC_VER
		#define COMPILER_MSVC	COMPILER_MSVC_2010
	#elif _MSC_VER == 1500
		#define COMPILER_MSVC_2008 _MSC_VER
		#define COMPILER_MSVC	COMPILER_MSVC_2008
	#else
		#define COMPILER_MSVC _MSC_VER
		#pragma warning "Unsupported Visual C++ compiler version"
	#endif
	#ifdef _NATIVE_NULLPTR_SUPPORTED
		#define COMPILER_SUPPORTS_CXX11
	#endif
#elif defined(__GNUG__)
	#define COMPILER_GCC
	#if (__GNUG__ >= 4) && (__GNUC_MINOR__ > 5)
		#define COMPILER_SUPPORTS_CXX11
	#endif
#else
	#pragma warning Unknown compiler
#endif

//---------------------------------------------------------------------//
// Build mode
//---------------------------------------------------------------------//

#if defined( DEBUG ) || defined( _DEBUG ) || defined( ___DEBUG )
#ifndef BUILD_DEBUG
	#define BUILD_DEBUG
#endif
#else
#ifndef BUILD_RELEASE
	#define BUILD_RELEASE
#endif
#endif

//---------------------------------------------------------------------//
// Compiler-specific functionality
//---------------------------------------------------------------------//

#define MULTI_LINE_MACRO_BEGIN do {

#if defined(COMPILER_MSVC)
	#if COMPILER_MSVC >= COMPILER_MSVC_2012
		#define _ALLOW_KEYWORD_MACROS
	#endif
	#define alignof _alignof
	#define thread_local __declspec(thread)
	#define ALIGN_BEGIN(size) __declspec(align(size))
	#define ALIGN_END(size)
	#define INLINE __forceinline
	#define NO_VTABLE __declspec(novtable)
	#define OVERRIDE override
	#pragma warning(disable: 4481) // override warning
	#define MULTI_LINE_MACRO_END \
		__pragma(warning(push)) \
		__pragma(warning(disable:4127)) \
		} while(0) \
		__pragma(warning(pop))
	#define FLD_BUILTIN_UNREACHABLE __assume(false)
#elif defined(COMPILER_GCC) || defined(COMPILER_CLANG)
	#define alignof __alignof__
	#define offsetof(type, member)  __builtin_offsetof (type, member)
	#define ALIGN_BEGIN(size)
	#define ALIGN_END(size) __attribute__((aligned(size)))
	#define INLINE __attribute__((always_inline))
	#define NO_VTABLE
	#define OVERRIDE //__attribute__((override))
	#define MULTI_LINE_MACRO_END } while(0)
	#define thread_local __thread
	#define FLD_BUILTIN_UNREACHABLE __builtin_unreachable()
#endif

#if defined(COMPILER_CLANG)
	// This is defined so MSVC headers work properly.
	#define _ALLOW_KEYWORD_MACROS
#endif

#if defined(PLATFORM_WINDOWS) && defined(COMPILER_MSVC)
	#define API_EXPORT __declspec(dllexport)
	#define API_IMPORT __declspec(dllimport)
	#define API_HIDDEN
#elif defined(PLATFORM_WINDOWS)
	#define API_EXPORT __attribute__ ((dllimport))
	#define API_IMPORT __attribute__ ((dllexport))
	#define API_HIDDEN
#else
	#define API_EXPORT __attribute__ ((visibility("default")))
	#define API_IMPORT __attribute__ ((visibility("default")))
	#define API_HIDDEN __attribute__ ((visibility("hidden")))
#endif

//---------------------------------------------------------------------//
// Basic types
//---------------------------------------------------------------------//

#include <cstdint>

typedef int8_t    int8;
typedef uint8_t   uint8;

typedef int16_t   int16;
typedef uint16_t  uint16;

typedef int32_t   int32;
typedef uint32_t  uint32;

// Transition types.
typedef uint8 byte;
typedef uint32 uint;

#if defined(COMPILER_MSVC)
	typedef __int64 int64;
	typedef unsigned __int64 uint64;
#elif defined(COMPILER_GCC) || defined(COMPILER_CLANG)
	typedef long long int64;
	typedef unsigned long long uint64;
#endif

//---------------------------------------------------------------------//
// Helper macros
//---------------------------------------------------------------------//

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#define __FILE__LINE__ __FILE__ "(" TOSTRING(__LINE__) ") : "
#define TODO( x )  message( __FILE__LINE__" TODO :   " #x ) 

#define FLD_ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

//---------------------------------------------------------------------//
// Error handling
//---------------------------------------------------------------------//

#include <cassert>

/// Marks that the current location is not supposed to be reachable.
/// Use this instead of assert(0).  It conveys intent more clearly and
/// allows compilers to omit some unnecessary code.
#ifdef BUILD_DEBUG
#define fld_unreachable(msg) \
    assert("Unreachable code: " # msg); \
    FLD_BUILTIN_UNREACHABLE
#else
#define fld_unreachable(msg) FLD_BUILTIN_UNREACHABLE
#endif

//---------------------------------------------------------------------//
// Forward-declaration Helpers
//---------------------------------------------------------------------//

#define FWD_DECL(T) NAMESPACE_CORE_BEGIN class T; NAMESPACE_CORE_END

//---------------------------------------------------------------------//
// Acessors
//---------------------------------------------------------------------//

#define GETTER(name, type, var) \
	type get##name() const { return var; }

#define SETTER(name, type, var) \
	void set##name(type v) { var = v; }

#define ACCESSOR(name, type, var) \
	GETTER(name, type, var) \
	SETTER(name, type, var)

#define DECLARE_UNCOPYABLE(Type) \
	private: \
		Type( const Type& ); \
		const Type& operator=( const Type& ); \
	public:

//---------------------------------------------------------------------//
// Binder macros
//---------------------------------------------------------------------//

#define FLD_VALUE_TYPE
#define FLD_OUT
#define FLD_IN_OUT
#define FLD_IGNORE
#define FLD_IGNORE_GEN
#define FLD_FLAGS
#define FLD_HASHCODE
#define FLD_EQUALS
#define FLD_READONLY
