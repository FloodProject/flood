/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

//---------------------------------------------------------------------//
// C++ foreach construct
//---------------------------------------------------------------------//

#ifdef VAPOR_COMPILER_MSVC
	#pragma warning( push )
	#pragma warning( disable : 4512 )
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
	#pragma warning( disable : 4100 )
	#pragma warning( disable : 4512 )
	//#pragma warning( disable : 1296 )
	#include <fd/delegate.hpp>
	#pragma warning( pop )
#else
	#include <fd/delegate.hpp>
#endif