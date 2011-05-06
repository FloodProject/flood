/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/Timer.h"

NAMESPACE_BEGIN

//-----------------------------------//

/**
 * Simple utility profiler class.
 */

class API_CORE Profiler
{
public:

	Profiler(const String& name);
	~Profiler();

protected:

	String name;
	Timer timer;
};

//-----------------------------------//

#define PROFILE Profiler p( __FUNCTION__ )
#define PROFILE_STR(s) Profiler p( __FUNCTION__ " / " s )

//-----------------------------------//

NAMESPACE_END