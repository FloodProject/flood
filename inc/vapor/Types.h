/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

//---------------------------------------------------------------------//
// Assert macros
//---------------------------------------------------------------------//

#include <cassert>

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

// TODO: assert all types are of the right size (sizeof ...)

//assert( sizeof( byte ) == 8 );
//assert( sizeof( ushort ) == 16 );
//assert( sizeof( uint ) == 32 );
//assert( sizeof( ulong ) == 32 );

#include <string>
#include <sstream>

//---------------------------------------------------------------------//
// Data structures
//---------------------------------------------------------------------//

#include <vector>
#include <list>
#include <map>
#include <tuple>
#include <stack>

//---------------------------------------------------------------------//
// C++0x compatibility macros
//---------------------------------------------------------------------//

// TODO: Check for VC2010

//#ifndef nullptr
//	#define nullptr NULL
//#endif