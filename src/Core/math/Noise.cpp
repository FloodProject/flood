/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"
#include "Math/Noise.h"
#include "Math/Helpers.h"
#include "Core/Log.h"

namespace vapor {

//-----------------------------------//

Noise::Noise( int seed )
	: seed(seed)
	, octaves(8)
	, frequency(4)
	, persistence(0.5)
	, lacunarity(2)
{ }

//-----------------------------------//

void Noise::generate(std::vector<float>& noise, uint width, uint height)
{
	assert( noise.empty() );

	noise.reserve(width*height);
	
	for (uint y = 0; y < height; y++)
	{
		for (uint x = 0; x < width; x++)
		{
			float total = 0;
			float freq = frequency;
			float amplitude = 1;
			float maxAmplitude = 0;

			for (uint i = 0; i < octaves; i++)
			{
				float fx = x * freq / (float)width;
				float fy = y * freq / (float)height;

				total += interpolateNoise(fx, fy) * amplitude;
				
				maxAmplitude += amplitude;
				
				freq *= lacunarity;
				amplitude *= persistence;
			}

			noise.push_back(total / maxAmplitude);
		}
	}
}

//-----------------------------------//

float Noise::interpolateNoise(float x, float y)
{
	int wholePartX = (int) x; // Same as flooring.
	float fractionPartX = x - wholePartX;

	int wholePartY = (int) y;
	float fractionPartY = y - wholePartY;

	float v1 = nextFloat(wholePartX, wholePartY);
	float v2 = nextFloat(wholePartX + 1, wholePartY);
	float v3 = nextFloat(wholePartX, wholePartY + 1);
	float v4 = nextFloat(wholePartX + 1, wholePartY + 1);

	float i1 = InterpolateCubic(v1, v2, fractionPartX);
	float i2 = InterpolateCubic(v3, v4, fractionPartX);

	return InterpolateCubic(i1, i2, fractionPartY);
}

//-----------------------------------//

float Noise::nextFloat(int x, int y)
{
	int n = x + y * 57 + seed;
	n = (n << 13) ^ n;
	n = ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff);

	return (float) (1.0 - n / 1073741824.0);
}

//-----------------------------------//

} // end namespace