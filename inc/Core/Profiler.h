/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/Timer.h"

NAMESPACE_CORE_BEGIN

//-----------------------------------//

/**
 * Simple utility profiler class.
 */

class API_CORE Profiler
{
public:

	Profiler(const char* category, const char* name);
	~Profiler();

	// Resets the profiler timer.
	void reset();

protected:

	const char* category;
	const char* name;
	Timer timer;
};

#define PROFILE_STR(s) Profiler p( s, __FUNCTION__ )

//-----------------------------------//

NAMESPACE_CORE_END