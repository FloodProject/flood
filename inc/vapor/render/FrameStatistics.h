/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

namespace vapor {

//-----------------------------------//

/**
 * Holds the timing statistics accumulated from rendering.
 */

class VAPOR_API FrameStatistics
{
public:

	FrameStatistics();

	// Updates the statistics for a new frame.
	void frameStep();

	// Gets the frames-per-second of the last frame.
	double getLastFPS() const;

//protected:

	uint64 numFrames;
	double minFrameTime;
	double maxFrameTime;
	double avgFrameTime;
	double sumFrameTime;
	double lastFrameTime;
};

//-----------------------------------//

} // end namespace
