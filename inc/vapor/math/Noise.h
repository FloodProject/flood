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
 * Generates coherent value noise (smooth pseudorandom noise).
 */

class API_CORE Noise
{
public:

	Noise( int seed = 329181337 );

	// Generates an array of noise.
	void generate(std::vector<float>& noise, uint width, uint height);

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

} // end namespace