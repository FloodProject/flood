/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

namespace vapor {

//-----------------------------------//

class Thread
{
};

//-----------------------------------//

} // end namespace

#ifdef VAPOR_THREADING
	#define THREAD( code ) code
#else
	#define THREAD( code )
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