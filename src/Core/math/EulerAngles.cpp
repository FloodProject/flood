/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core.h"
#include "vapor/math/EulerAngles.h"

namespace vapor {

//-----------------------------------//

EulerAngles::EulerAngles()
	: x(0.0f)
	, y(0.0f)
	, z(0.0f)
{ }

//-----------------------------------//

EulerAngles::EulerAngles( float x, float y, float z )
	: x(x)
	, y(y)
	, z(z)
{ }

//-----------------------------------//

EulerAngles::EulerAngles( const EulerAngles& ang )
	: x(ang.x)
	, y(ang.y)
	, z(ang.z)
{ }

//-----------------------------------//

EulerAngles& EulerAngles::operator += (const EulerAngles& ang)
{
	x += ang.x;
	y += ang.y;
	z += ang.z;

	return *this;
}

//-----------------------------------//

EulerAngles& EulerAngles::operator += (const Vector3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;

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

} // end namespace