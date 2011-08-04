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

void Profiler::reset()
{
	TimerReset(&timer);
}

//-----------------------------------//

Profiler::~Profiler()
{
	float time = TimerGetElapsed(&timer);
	float ms = time * 1000;

	LogDebug( "%s: took %lu ms (%.5lf)", name, (uint32) ms, ms );
}

//-----------------------------------//

NAMESPACE_CORE_END