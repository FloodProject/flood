/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/Timer.h"

namespace vapor {

//-----------------------------------//

/**
 * Simple utility profiler class.
 */

class VAPOR_API Profiler
{
public:
	Profiler(const std::string& name);
	~Profiler();

private:

	Timer time;
	std::string name;
};

//-----------------------------------//

#define PROFILE Profiler p( __FUNCTION__ )
#define PROFILE_STR(s) Profiler p( __FUNCTION__ " / " s )

//-----------------------------------//

} // end namespace 