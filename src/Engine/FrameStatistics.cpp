/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/render/FrameStatistics.h"

namespace vapor {

//-----------------------------------//

FrameStatistics::FrameStatistics()
	: numFrames(0)
	, minFrameTime( std::numeric_limits<double>::max() )
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

double FrameStatistics::getLastFPS() const
{
	if(lastFrameTime == 0)
		return 0;
	
	return 1.0 / lastFrameTime;
}

//-----------------------------------//

} // end namespace