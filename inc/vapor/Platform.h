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
	#define VAPOR_PLATFORM_WINDOWS
#elif defined(__APPLE__) || defined(MACOSX)
	#define VAPOR_PLATFORM_MACOSX
#elif defined(__LINUX) || defined(__LINUX__) || defined(LINUX)
	#define VAPOR_PLATFORM_LINUX
#else
	#warn Unknown platform
#endif

//---------------------------------------------------------------------//
// Compiler detection
//---------------------------------------------------------------------//

#ifdef _MSC_VER // Microsoft Visual C++ Compiler
	#if _MSC_VER == 1600
		#define VAPOR_COMPILER_MSVC_2010 0x1600
		#define VAPOR_COMPILER_MSVC	VAPOR_COMPILER_MSVC_2010
	#elif _MSC_VER == 1500
		#define VAPOR_COMPILER_MSVC_2008 0x1500
		#define VAPOR_COMPILER_MSVC	VAPOR_COMPILER_MSVC_2008
	#else
		#error "Unknown Visual C++ compiler version"
	#endif
#elif defined(__GNUG__)
	// GCC
	#define VAPOR_COMPILER_GCC
#else
	#warn Unknown compiler
#endif

//---------------------------------------------------------------------//
// Build mode
//---------------------------------------------------------------------//

#if defined( DEBUG ) || defined( _DEBUG ) || defined( ___DEBUG )
	#define VAPOR_DEBUG
#elif !defined( VAPOR_DEBUG ) && !defined( NDEBUG )
	#define NDEBUG
#elif defined( VAPOR_DEBUG ) && defined( NDEBUG )
	#error Conflicting debug options
#endif

//---------------------------------------------------------------------//
// Alignment control
//---------------------------------------------------------------------//

#if defined(VAPOR_COMPILER_MSVC)
	#define VAPOR_ALIGN_BEGIN(size) __declspec(align(size))
	#define VAPOR_ALIGN_END(size)
	#define VAPOR_INLINE __forceinline
	#define VAPOR_PURE __declspec(novtable)
#elif defined(VAPOR_COMPILER_GCC)
	#define VAPOR_ALIGN_BEGIN(size)
	#define VAPOR_ALIGN_END(size) __attribute__((aligned(size)))
	#define VAPOR_INLINE __attribute__((always_inline))
	#define VAPOR_PURE
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

#if defined(VAPOR_COMPILER_MSVC)
	typedef __int64 int64;
	typedef signed   __int64 sint64;
	typedef unsigned __int64 uint64;
#elif defined(VAPOR_COMPILER_GCC)
	typedef long long int64;
	typedef unsigned long long uint64;
#endif

#ifndef SWIG
static_assert(sizeof(int8)  == 1, "");
static_assert(sizeof(int16) == 2, "");
static_assert(sizeof(int32) == 4, "");
static_assert(sizeof(int64) == 8, "");
#endif

//---------------------------------------------------------------------//
// nullptr replacement
//---------------------------------------------------------------------//

#if !defined(VAPOR_COMPILER_MSVC) || (VAPOR_COMPILER_MSVC != VAPOR_COMPILER_MSVC_2010)
	#define nullptr NULL
#endif

//---------------------------------------------------------------------//
// Helper macros to stringify parameters.
//---------------------------------------------------------------------//

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#define __FILE__LINE__ __FILE__ "(" TOSTRING(__LINE__) ") : "
#define TODO( x )  message( __FILE__LINE__" TODO :   " #x ) 

//---------------------------------------------------------------------//
// Array and Conversion Helpers
//---------------------------------------------------------------------//

#define VAPOR_ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

//---------------------------------------------------------------------//
// Types and Data structures
//---------------------------------------------------------------------//

#include <map>
#include <vector>
#include <cassert>

//---------------------------------------------------------------------//
// Forward-declaration Helpers
//---------------------------------------------------------------------//

#define FWD_DECL(T) namespace vapor { class T; }				\

#define TYPEDEF_SHARED_POINTER_FROM_TYPE( class )				\
	typedef std::shared_ptr< class > class##Ptr;

#define TYPEDEF_SHARED_WEAK_POINTER_FROM_TYPE( class )			\
		typedef std::weak_ptr< class > class##WeakPtr;

#define FWD_DECL_SHARED(T)										\
	namespace vapor {											\
		class T;												\
		TYPEDEF_SHARED_POINTER_FROM_TYPE( T );					\
	} // end namespace

#define FWD_DECL_SHARED_WEAK(T)									\
	namespace vapor {											\
		class T;												\
		TYPEDEF_SHARED_WEAK_POINTER_FROM_TYPE( T );				\
	} // end namespace

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