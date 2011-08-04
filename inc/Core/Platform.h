/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
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
#else
	#warn Unknown platform
#endif

//---------------------------------------------------------------------//
// Compiler detection
//---------------------------------------------------------------------//

#ifdef _MSC_VER
	#if _MSC_VER == 1600
		#define COMPILER_MSVC_2010 0x1600
		#define COMPILER_MSVC	COMPILER_MSVC_2010
		#define COMPILER_SUPPORTS_CPP0X
	#elif _MSC_VER == 1500
		#define COMPILER_MSVC_2008 0x1500
		#define COMPILER_MSVC	COMPILER_MSVC_2008
	#else
		#error "Unknown Visual C++ compiler version"
	#endif
#elif defined(__GNUG__)
	#define COMPILER_GCC
#elif defined(__clang__)
	#define COMPILER_CLANG
#else
	#warn Unknown compiler
#endif

//---------------------------------------------------------------------//
// Build mode
//---------------------------------------------------------------------//

#if defined( DEBUG ) || defined( _DEBUG ) || defined( ___DEBUG )
	#define BUILD_DEBUG
	#define DEBUG_BUILD
#else
	#define BUILD_RELEASE
	#define RELEASE_BUILD
#endif

//---------------------------------------------------------------------//
// Compiler-specific functionality
//---------------------------------------------------------------------//

#if defined(COMPILER_MSVC)
	#define alignof _alignof
	#define ALIGN_BEGIN(size) __declspec(align(size))
	#define ALIGN_END(size)
	#define INLINE __forceinline
	#define NO_VTABLE __declspec(novtable)
	#define OVERRIDE override
	#pragma warning(disable: 4481) // override warning
#elif defined(COMPILER_GCC) || defined(COMPILER_CLANG)
	#define alignof __alignof__ 
	#define ALIGN_BEGIN(size)
	#define ALIGN_END(size) __attribute__((aligned(size)))
	#define INLINE __attribute__((always_inline))
	#define NO_VTABLE
	#define OVERRIDE //__attribute__((override))
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

typedef char			int8;
typedef signed char		sint8;
typedef unsigned char	uint8;

typedef short			int16;
typedef signed short	sint16;
typedef unsigned short	uint16;

typedef long			int32;
typedef signed long		sint32;
typedef unsigned long	uint32;

// Transition types.
typedef uint8 byte;
typedef uint32 uint;

#if defined(COMPILER_MSVC)
	typedef __int64 int64;
	typedef signed   __int64 sint64;
	typedef unsigned __int64 uint64;
#elif defined(COMPILER_GCC)
	typedef long long int64;
	typedef unsigned long long uint64;
#endif

#ifndef SWIG
static_assert(sizeof(int8)  == 1, "");
static_assert(sizeof(int16) == 2, "");
static_assert(sizeof(int32) == 4, "");
static_assert(sizeof(int64) == 8, "");
#endif

#if defined(COMPILER_MSVC)
	#define enum_class enum
#elif defined(COMPILER_GCC)
	#define enum_class enum class
#endif

#if !defined(COMPILER_SUPPORTS_CPP0X)
	#define nullptr NULL
#endif

//---------------------------------------------------------------------//
// Helper macros
//---------------------------------------------------------------------//

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#define __FILE__LINE__ __FILE__ "(" TOSTRING(__LINE__) ") : "
#define TODO( x )  message( __FILE__LINE__" TODO :   " #x ) 

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

//---------------------------------------------------------------------//
// Types and Data structures
//---------------------------------------------------------------------//

#include <map>
#include <vector>
#include <cassert>

//---------------------------------------------------------------------//
// Forward-declaration Helpers
//---------------------------------------------------------------------//

#define FWD_DECL(T) NAMESPACE_CORE_BEGIN class T; NAMESPACE_CORE_END		\

#ifdef ENABLE_MEMORY_SHARED_PTR

#define TYPEDEF_SHARED_POINTER_FROM_TYPE( class )				\
	typedef std::shared_ptr< class > class##Ptr;

#define TYPEDEF_SHARED_WEAK_POINTER_FROM_TYPE( class )			\
		typedef std::weak_ptr< class > class##WeakPtr;

#define FWD_DECL_SHARED(T)										\
	NAMESPACE_CORE_BEGIN												\
		class T;												\
		TYPEDEF_SHARED_POINTER_FROM_TYPE( T );					\
	NAMESPACE_CORE_END

#define FWD_DECL_SHARED_WEAK(T)									\
	NAMESPACE_CORE_BEGIN												\
		class T;												\
		TYPEDEF_SHARED_WEAK_POINTER_FROM_TYPE( T );				\
	NAMESPACE_CORE_END

#endif

//---------------------------------------------------------------------//
// Acessors
//---------------------------------------------------------------------//

#define GETTER(name, type, var)					\
	type get##name() const { return var; }

#define SETTER(name, type, var)					\
	void set##name(type v) { var = v; }

#define ACESSOR(name, type, var)				\
	GETTER(name, type, var)						\
	SETTER(name, type, var)

#define DECLARE_UNCOPYABLE(Type)				\
	private:									\
		Type( const Type& );					\
		const Type& operator=( const Type& );	\
	public: