/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------//

/**
 * Holds the timing statistics accumulated from rendering.
 */

class API_GRAPHICS FrameStatistics
{
public:

	FrameStatistics();

	// Updates the statistics for a new frame.
	void frameStep();

	// Gets the frames-per-second of the last frame.
	float getLastFPS() const;

//protected:

	uint64 numFrames;
	float minFrameTime;
	float maxFrameTime;
	float avgFrameTime;
	float sumFrameTime;
	float lastFrameTime;
};

//-----------------------------------//

NAMESPACE_GRAPHICS_END
