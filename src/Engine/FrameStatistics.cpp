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
	: numFrames(0),
	minFrameTime( std::numeric_limits<double>::max() ),
	maxFrameTime(0),
	avgFrameTime(0),
	sumFrameTime(0),
	lastFrameTime(0)
{ }

//-----------------------------------//

double FrameStatistics::getLastFPS() const
{
	if(lastFrameTime == 0)
		return 0;
	
	return 1.0 / lastFrameTime;
}

//-----------------------------------//

} // end namespace