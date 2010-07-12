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
#include <boost/noncopyable.hpp>

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
// Delegates
//---------------------------------------------------------------------//

#ifdef VAPOR_COMPILER_MSVC
	#pragma warning( push )
	#pragma warning( disable : 4100 4512 4702 )
	#include <fd/delegate.hpp>
	#include <fd/delegate/bind.hpp>
	#pragma warning( pop )
#else
	#include <fd/delegate.hpp>
	#include <fd/delegate/bind.hpp>
#endif

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

#define TYPEDEF_PTR(type)		\
	typedef type* type##Ptr;

#define TYPEDEF_AUTO_PTR(type)	\
	typedef std::auto_ptr<type> type##Ptr;

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
	using boost::static_pointer_cast;
#endif

//---------------------------------------------------------------------//
// Iterator Debugging
//---------------------------------------------------------------------//

#ifdef VAPOR_COMPILER_MSVC
	#ifndef D_SCL_SECURE_NO_WARNINGS
		#define	D_SCL_SECURE_NO_WARNINGS
	#endif
	
	#ifndef _SECURE_SCL
		#define _SECURE_SCL 0
	#endif
	
	#ifndef _HAS_ITERATOR_DEBUGGING
		#define _HAS_ITERATOR_DEBUGGING 0
	#endif
#endif