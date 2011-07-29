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

Profiler::Profiler(const String& name)
	: name(name)
{
	TimerReset(&timer);
}

//-----------------------------------//

Profiler::~Profiler()
{
	float time = TimerGetElapsed(&timer);
	float ms = time * 1000;

	LogDebug( "%s: took %lu ms (%.5lf)", name.c_str(), (uint32) ms, ms );
}

//-----------------------------------//

NAMESPACE_CORE_END