/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Core/API.h"
#include "Core/Profiler.h"
#include "Core/Log.h"

NAMESPACE_CORE_BEGIN

//-----------------------------------//

Profiler::Profiler(const char* category, const char* name)
	: category(category)
	, name(name)
{
	reset();
}

//-----------------------------------//

Profiler::~Profiler()
{
	float time = timer.getElapsed();
	float ms = time * 1000;

	LogDebug( "%s: took %lu ms (%.5lf)", name, (uint32) ms, ms );
}

//-----------------------------------//

void Profiler::reset()
{
	timer.reset();
}

//-----------------------------------//

NAMESPACE_CORE_END