/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

/**
 * Holds the timing statistics accumulated from rendering.
 */

class API_ENGINE FrameStatistics
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

NAMESPACE_ENGINE_END
