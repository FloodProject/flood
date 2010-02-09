/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/math/EulerAngles.h"
#include "vapor/math/Math.h"

namespace vapor { namespace math {

//-----------------------------------//

EulerAngles::EulerAngles()
	: xang( 0.0f ), yang( 0.0f ), zang( 0.0f )
{

}

//-----------------------------------//

EulerAngles::EulerAngles( float x, float y, float z )
	: xang( x ), yang( y ), zang( z )
{

}

//-----------------------------------//

void EulerAngles::rotate( float x, float y, float z )
{
	xang += x;
	yang += y;
	zang += z;
}

//-----------------------------------//

void EulerAngles::canonize()
{
	//const float LIMIT = math::degreeToRadian( 90.0f );

	//if( xang >= LIMIT ) xang -= LIMIT;
	//if( xang <= -LIMIT ) xang += LIMIT;
	//
	//if( yang >= LIMIT ) yang -= LIMIT;
	//if( yang <= -LIMIT ) yang += LIMIT;

	//if( zang >= LIMIT ) zang -= LIMIT;
	//if( zang <= -LIMIT ) zang += LIMIT;

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
	xang = yang = zang = 0;
}

//-----------------------------------//

math::Matrix4x3 EulerAngles::rotateX( float ang ) const
{
	const float cos = math::cosf( degreeToRadian(ang) );
	const float sin = math::sinf( degreeToRadian(ang) );

	Matrix4x3 newRotation;

	newRotation.m11 = 1;

	newRotation.m22 = cos;
	newRotation.m23 = sin;

	newRotation.m32 = -sin;
	newRotation.m33 = cos;

	return newRotation;
}

//-----------------------------------//

math::Matrix4x3 EulerAngles::rotateY( float ang ) const
{
	const float cos = math::cosf( degreeToRadian(ang) );
	const float sin = math::sinf( degreeToRadian(ang) );

	Matrix4x3 newRotation;

	newRotation.m11 = cos;
	newRotation.m13 = -sin;

	newRotation.m22 = 1;

	newRotation.m31 = sin;
	newRotation.m33 = cos;

	return newRotation;
}

//-----------------------------------//

math::Matrix4x3 EulerAngles::rotateZ( float ang ) const
{
	const float cos = math::cosf( degreeToRadian(ang) );
	const float sin = math::sinf( degreeToRadian(ang) );

	Matrix4x3 newRotation;

	newRotation.m11 = cos;
	newRotation.m12 = sin;

	newRotation.m21 = -sin;
	newRotation.m22 = cos;

	newRotation.m33 = 1;

	return newRotation;
}

//-----------------------------------//

math::Matrix4x3 EulerAngles::getOrientationMatrix() const
{
	return rotateX( xang ) * rotateY( yang ) * rotateZ( zang );
}

//-----------------------------------//

} } // end namespaces