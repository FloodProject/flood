/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

//---------------------------------------------------------------------//
// Basic types
//---------------------------------------------------------------------//

typedef unsigned char	byte;
typedef unsigned short	ushort;
typedef unsigned int	uint;
typedef unsigned long	ulong;

#if defined(VAPOR_COMPILER_MSVC)
	typedef __int64 int64;
	typedef unsigned __int64 uint64;
#elif defined(VAPOR_COMPILER_GCC)
	typedef long long int64;
	typedef unsigned long long uint64;
#endif

// TODO: assert all types are of the right size

//assert( sizeof( byte ) == 8 );
//assert( sizeof( ushort ) == 16 );
//assert( sizeof( uint ) == 32 );
//assert( sizeof( ulong ) == 32 );

//---------------------------------------------------------------------//
// Types and Data structures
//---------------------------------------------------------------------//

#include <vector>
#include <list>
#include <map>
#include <deque>
#include <stack>
#include <queue>
#include <bitset>
#include <string>
#include <sstream>

//---------------------------------------------------------------------//
// Miscellaneous headers
//---------------------------------------------------------------------//

#include <cstdio>
#include <cstdarg>
#include <ctime>
#include <cassert>
#include <algorithm>
#include <limits>
#include <iostream>

//---------------------------------------------------------------------//
// nullptr replacement
//---------------------------------------------------------------------//

#ifdef VAPOR_COMPILER_MSVC
	#if VAPOR_COMPILER_MSVC != VAPOR_COMPILER_MSVC_2010 
		#define nullptr NULL
	#endif
#endif

//---------------------------------------------------------------------//
// C++ foreach construct
//---------------------------------------------------------------------//

#ifdef VAPOR_COMPILER_MSVC
	#pragma warning( push )
	#pragma warning( disable : 4512 4503 )
	#include <boost/foreach.hpp>
	#pragma warning( pop )
#else
	#include <boost/foreach.hpp>
#endif

#define foreach         BOOST_FOREACH
#define reverse_foreach BOOST_REVERSE_FOREACH

//---------------------------------------------------------------------//
// Threads
//---------------------------------------------------------------------//

#ifdef VAPOR_THREADING
	#define THREAD( code ) code
#else
	#define THREAD( code )
	namespace vapor { class Thread; }
#endif

#ifdef VAPOR_THREADING_BOOST
	#define BOOST_THREAD_NO_LIB

	#ifdef VAPOR_COMPILER_MSVC
		#pragma warning( push )
		#pragma warning( disable : 4512 4244 )
		#include <boost/thread.hpp>
		#pragma warning( pop )
	#else
		#include <boost/thread.hpp>
	#endif

	typedef boost::thread Thread;
#endif

#if !defined(VAPOR_THREADING)
	typedef int atomic_int;
#elif defined(VAPOR_THREADING) && defined(VAPOR_THREADING_BOOST)
	#include <boost/atomic.hpp>
	typedef boost::atomic<int> atomic_int;
#elif defined(VAPOR_THREADING) && defined(VAPOR_THREADING_STD)
	typedef std::atomic<int> atomic_int;
#else
	#error
#endif

//---------------------------------------------------------------------//
// Pointer wrappers
//---------------------------------------------------------------------//

#if defined(VAPOR_MEMORY_TR1_VENDOR)
	#if defined(VAPOR_COMPILER_MSVC)
		#include <memory>
		#include <functional>
	#elif defined(VAPOR_COMPILER_GCC)
		#include <tr1/memory>
	#endif
#elif defined(VAPOR_MEMORY_TR1_BOOST)
	#include <boost/tr1/memory.hpp>
#endif

#ifdef VAPOR_COMPILER_MSVC
	#if VAPOR_COMPILER_MSVC != VAPOR_COMPILER_MSVC_2010 
		namespace std { using namespace std::tr1; }
	#endif
#endif

#if defined( VAPOR_MEMORY_SHARED_PTR )
	#define TYPEDEF_SHARED_POINTER_FROM_TYPE( class ) \
		typedef std::shared_ptr< class > class##Ptr;
	#define TYPEDEF_SHARED_WEAK_POINTER_FROM_TYPE( class ) \
		typedef std::weak_ptr< class > class##WeakPtr;
#endif

#if defined( VAPOR_MEMORY_INTRUSIVE_PTR )
	#include "boost/intrusive_ptr.hpp"
	#define TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( type ) \
		typedef boost::intrusive_ptr< type > type##Ptr;
#endif

//---------------------------------------------------------------------//
// Macro for unused parameters to clean up compiler warnings
//---------------------------------------------------------------------//

#define VAPOR_UNUSED( id )

//---------------------------------------------------------------------//
// Helper macros to stringify parameters.
//---------------------------------------------------------------------//

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#define __FILE__LINE__ __FILE__ "(" TOSTRING(__LINE__) ") : "
#define TODO( x )  message( __FILE__LINE__" TODO :   " #x "\n" ) 

//---------------------------------------------------------------------//
// Array and Conversion Helpers
//---------------------------------------------------------------------//

#define VAPOR_ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

//---------------------------------------------------------------------//
// Forward-declaration Helpers
//---------------------------------------------------------------------//

#define FWD_DECL_INTRUSIVE(T)						\
	namespace vapor {								\
		class T;									\
		TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( T );	\
	} // end namespace
	
#define FWD_DECL_SHARED(T)							\
	namespace vapor {								\
		class T;									\
		TYPEDEF_SHARED_POINTER_FROM_TYPE( T );		\
	} // end namespace

#define FWD_DECL_SHARED_WEAK(T)						\
	namespace vapor {								\
		class T;									\
		TYPEDEF_SHARED_WEAK_POINTER_FROM_TYPE( T );	\
	} // end namespace

//---------------------------------------------------------------------//
// Acessors
//---------------------------------------------------------------------//

#define DECLARE_GETTER(name, type)				\
	type get##name() const;

#define DECLARE_SETTER(name, type)				\
	void set##name(type v);

#define DECLARE_ACESSOR(name, type)				\
	DECLARE_GETTER(name, type)					\
	DECLARE_SETTER(name, type)

#define GETTER(name, type, var)					\
	type get##name() const { return var; }

#define SETTER(name, type, var)					\
	void set##name(type v) { var = v; }

#define GETTER_PTR(name, type, var)				\
	type get##name() const { return *var; }

#define SETTER_PTR(name, type, var)				\
	void set##name(type v) { var = &v; }

#define ACESSOR(name, type, var)				\
	GETTER(name, type, var)						\
	SETTER(name, type, var)

#define ACESSOR_PTR(name, type, var)			\
	GETTER_PTR(name, type, var)					\
	SETTER_PTR(name, type, var)

#ifdef VAPOR_COMPILER_MSVC
	#define __stricmp _stricmp
#else
	#define __stricmp stricmp
#endif

#define DECLARE_UNCOPYABLE(Type)				\
	private:									\
		Type( const Type& );					\
		const Type& operator=( const Type& );
