/************************************************************************
*
* Flood Project � (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

NAMESPACE_CORE_BEGIN

//-----------------------------------//

/**
 * Generates coherent value noise (smooth pseudorandom noise).
 */

struct API_CORE Noise
{
	Noise( int seed = 329181337 );

	// Generates an array of noise.
	void generate(Array<float>& noise, uint width, uint height);

	// Interpolates the noise.
	float interpolateNoise(float x, float y);

	// Returns a pseudo-random float.
	float nextFloat(int x, int y);

	int seed;
	uint octaves;
	float frequency;
	float persistence;
	float lacunarity;
};

//-----------------------------------//

NAMESPACE_CORE_END