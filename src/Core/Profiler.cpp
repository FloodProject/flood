/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
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
	float time = TimerGetElapsed(&timer);
	float ms = time * 1000;

	LogDebug( "%s: took %lu ms (%.5lf)", name, (uint32) ms, ms );
}

//-----------------------------------//

void Profiler::reset()
{
	TimerReset(&timer);
}

//-----------------------------------//

NAMESPACE_CORE_END