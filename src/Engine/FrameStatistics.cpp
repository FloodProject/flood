/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"
#include "Graphics/FrameStatistics.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

FrameStatistics::FrameStatistics()
	: numFrames(0)
	, minFrameTime( std::numeric_limits<float>::max() )
	, maxFrameTime(0)
	, avgFrameTime(0)
	, sumFrameTime(0)
	, lastFrameTime(0)
{ }

//-----------------------------------//

void FrameStatistics::frameStep()
{
	numFrames++;

	minFrameTime = std::min( minFrameTime, lastFrameTime );
	maxFrameTime = std::max( maxFrameTime, lastFrameTime );

	sumFrameTime += lastFrameTime;
	avgFrameTime = sumFrameTime / numFrames;
}

//-----------------------------------//

float FrameStatistics::getLastFPS() const
{
	if(lastFrameTime == 0)
		return 0;
	
	return 1.0f / lastFrameTime;
}

//-----------------------------------//

NAMESPACE_ENGINE_END