/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/math/EulerAngles.h"
#include "vapor/math/Math.h"

namespace vapor {

//-----------------------------------//

EulerAngles::EulerAngles()
	: x(0.0f),
	y(0.0f),
	z(0.0f)
{ }

//-----------------------------------//

EulerAngles::EulerAngles( float x, float y, float z )
	: x(x),
	y(y),
	z(z)
{ }

//-----------------------------------//

EulerAngles::EulerAngles( const EulerAngles& ang )
	: x(ang.x), y(ang.y), z(ang.z)
{ }

//-----------------------------------//

EulerAngles& EulerAngles::operator += (const EulerAngles& ang)
{
	x += ang.x; y += ang.y; z += ang.z;
	return *this;
}

//-----------------------------------//

EulerAngles& EulerAngles::operator += (const Vector3& v)
{
	x += v.x; y += v.y; z += v.z;
	return *this;
}


//-----------------------------------//

void EulerAngles::rotate( float x, float y, float z )
{
	x += x;
	y += y;
	z += z;
}

//-----------------------------------//

void EulerAngles::canonize()
{
	//const float LIMIT = degreeToRadian( 90.0f );

	//if( x >= LIMIT ) x -= LIMIT;
	//if( x <= -LIMIT ) x += LIMIT;
	//
	//if( y >= LIMIT ) y -= LIMIT;
	//if( y <= -LIMIT ) y += LIMIT;

	//if( z >= LIMIT ) z -= LIMIT;
	//if( z <= -LIMIT ) z += LIMIT;

	// First, wrap pitch in range -pi ... pi

	//pitch = wrapPi(pitch);

	//// Now, check for "the back side" of the matrix, pitch outside
	//// the canonical range of -pi/2 ... pi/2

	//if (pitch < -kPiOver2) {
	//	pitch = -kPi - pitch;
	//	heading += kPi;
	//	bank += kPi;
	//} else if (pitch > kPiOver2) {
	//	pitch = kPi - pitch;
	//	heading += kPi;
	//	bank += kPi;
	//}

	//// OK, now check for the gimbel lock case (within a slight
	//// tolerance)

	//if (fabs(pitch) > kPiOver2 - 1e-4) {

	//	// We are in gimbel lock.  Assign all rotation
	//	// about the vertical axis to heading

	//	heading += bank;
	//	bank = 0.0f;

	//} else {

	//	// Not in gimbel lock.  Wrap the bank angle in
	//	// canonical range 

	//	bank = wrapPi(bank);
	//}

	//// Wrap heading in canonical range

	//heading = wrapPi(heading);
}

//-----------------------------------//

void EulerAngles::identity()
{
	x = y = z = 0;
}

//-----------------------------------//

Matrix4x3 EulerAngles::rotateX( float ang ) const
{
	const float cos = cosf( degreeToRadian(ang) );
	const float sin = sinf( degreeToRadian(ang) );

	Matrix4x3 newRotation;

	newRotation.m11 = 1;

	newRotation.m22 = cos;
	newRotation.m23 = sin;

	newRotation.m32 = -sin;
	newRotation.m33 = cos;

	return newRotation;
}

//-----------------------------------//

Matrix4x3 EulerAngles::rotateY( float ang ) const
{
	const float cos = cosf( degreeToRadian(ang) );
	const float sin = sinf( degreeToRadian(ang) );

	Matrix4x3 newRotation;

	newRotation.m11 = cos;
	newRotation.m13 = -sin;

	newRotation.m22 = 1;

	newRotation.m31 = sin;
	newRotation.m33 = cos;

	return newRotation;
}

//-----------------------------------//

Matrix4x3 EulerAngles::rotateZ( float ang ) const
{
	const float cos = cosf( degreeToRadian(ang) );
	const float sin = sinf( degreeToRadian(ang) );

	Matrix4x3 newRotation;

	newRotation.m11 = cos;
	newRotation.m12 = sin;

	newRotation.m21 = -sin;
	newRotation.m22 = cos;

	newRotation.m33 = 1;

	return newRotation;
}

//-----------------------------------//

Matrix4x3 EulerAngles::getOrientationMatrix() const
{
	return rotateX( x ) * rotateY( y ) * rotateZ( z );
}

//-----------------------------------//

} // end namespace