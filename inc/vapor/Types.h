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

// TODO: assert all types are of the right size (sizeof ...)

//assert( sizeof( byte ) == 8 );
//assert( sizeof( ushort ) == 16 );
//assert( sizeof( uint ) == 32 );
//assert( sizeof( ulong ) == 32 );

#include <string>

//---------------------------------------------------------------------//
// Data structures
//---------------------------------------------------------------------//

#include <vector>
#include <list>
#include <map>
#include <tuple>

//---------------------------------------------------------------------//
// C++0x compatability macros
//---------------------------------------------------------------------//

#define nullptr NULL