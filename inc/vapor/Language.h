/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

//---------------------------------------------------------------------//
// C++ min/max templates and numeric limits
//---------------------------------------------------------------------//

#include <algorithm>
#include <limits>

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
// Non-copyable construct
//---------------------------------------------------------------------//

#include <boost/utility.hpp>

//---------------------------------------------------------------------//
// Macro for unused parameters to clean up 'The Warning Disease'
//---------------------------------------------------------------------//

#define VAPOR_UNUSED( id )

#ifndef UNUSED
	#define UNUSED VAPOR_UNUSED
#endif

//---------------------------------------------------------------------//
// nullptr replacement
//---------------------------------------------------------------------//

#ifdef VAPOR_COMPILER_MSVC
	#if VAPOR_COMPILER_MSVC != VAPOR_COMPILER_MSVC_2010 
		#define nullptr NULL
	#endif
#endif