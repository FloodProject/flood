/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"
#include "Math/Quaternion.h"
#include "Core/Log.h"
#include <cassert>

NAMESPACE_CORE_BEGIN

//-----------------------------------//

const Quaternion Quaternion::Identity;

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

EulerAngles Quaternion::getEulerAngles() const
{
	float pitch, heading, bank;

	// Extract sin(pitch)
	float sp = -2.0f * (y*z + w*x);

	// Check for Gimbel lock, giving slight tolerance for numerical imprecision
	if (fabs(sp) > 0.9999f)
	{
		// Looking straight up or down
		pitch = (PI / 2.0f) * sp;

		// Compute heading, slam bank to zero
		heading = atan2(-x*z - w*y, 0.5f - y*y - z*z);
		bank = 0.0f;
	}
	else
	{
		// Compute angles. We don't have to use the "safe" asin
		// function because we already checked for range errors when
		// checking for Gimbal lock
		pitch	= asin(sp);
		heading	= atan2(x*z - w*y, 0.5f - x*x - y*y);
		bank	= atan2(x*y - w*z, 0.5f - x*x - z*z);
	}

	return EulerAngles(
		MathRadianToDegree(pitch),
		MathRadianToDegree(heading),
		MathRadianToDegree(bank));
}

//-----------------------------------//

NAMESPACE_CORE_END
