/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core.h"
#include "math/Quaternion.h"
#include "Log.h"

namespace vapor {

//-----------------------------------//

const Quaternion Quaternion::Identity;

//-----------------------------------//

Quaternion Quaternion::slerp(const Quaternion& q, float t)
{
	// Check for out-of range parameter and return edge points if so
	if (t <= 0.0f)
		return *this;

	if (t >= 1.0f)
		return q;

	// Compute "cosine of angle between quaternions" using dot product
	float cosOmega = dot(q);

	// If negative dot, use -q.  Two quaternions q and -q
	// represent the same rotation, but may produce
	// different slerp.  We chose q or -q to rotate using
	// the acute angle.
	float q1w = q.w;
	float q1x = q.x;
	float q1y = q.y;
	float q1z = q.z;

	if (cosOmega < 0.0f)
	{
		q1w = -q1w;
		q1x = -q1x;
		q1y = -q1y;
		q1z = -q1z;
		cosOmega = -cosOmega;
	}

	// We should have two unit quaternions, so dot should be <= 1.0
	assert(cosOmega < 1.1f);

	// Compute interpolation fraction, checking for quaternions
	// almost exactly the same
	float k0, k1;
	if (cosOmega > 0.9999f)
	{
		// Very close - just use linear interpolation,
		// which will protect againt a divide by zero

		k0 = 1.0f-t;
		k1 = t;
	}
	else
	{
		// Compute the sin of the angle using the
		// trig identity sin^2(omega) + cos^2(omega) = 1
		float sinOmega = sqrt(1.0f - cosOmega*cosOmega);

		// Compute the angle from its sin and cosine
		float omega = atan2(sinOmega, cosOmega);

		// Compute inverse of denominator, so we only have
		// to divide once
		float oneOverSinOmega = 1.0f / sinOmega;

		// Compute interpolation parameters
		k0 = sin((1.0f - t) * omega) * oneOverSinOmega;
		k1 = sin(t * omega) * oneOverSinOmega;
	}

	// Interpolate
	Quaternion result;
	result.x = k0*x + k1*q1x;
	result.y = k0*y + k1*q1y;
	result.z = k0*z + k1*q1z;
	result.w = k0*w + k1*q1w;

	return result;
}

//-----------------------------------//

} // end namespace
