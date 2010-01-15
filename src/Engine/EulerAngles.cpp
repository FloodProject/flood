/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#include "vapor/math/EulerAngles.h"

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

}

//-----------------------------------//

void EulerAngles::identity()
{
	xang = yang = zang = 0;
}

//-----------------------------------//

math::Matrix4x3 EulerAngles::rotateX( float ang ) const
{
	Matrix4x3 newRotation;

	newRotation.m11 = 1;

	newRotation.m22 = math::cosf( ang );
	newRotation.m23 = math::sinf( ang );

	newRotation.m32 = -math::sinf( ang );
	newRotation.m33 = math::cosf( ang );

	return newRotation;
}

//-----------------------------------//

math::Matrix4x3 EulerAngles::rotateY( float ang ) const
{
	Matrix4x3 newRotation;

	newRotation.m11 = math::cosf( ang );
	newRotation.m13 = -math::sinf( ang );

	newRotation.m22 = 1;

	newRotation.m31 = math::sinf( ang );
	newRotation.m33 = math::cosf( ang );

	return newRotation;
}

//-----------------------------------//

math::Matrix4x3 EulerAngles::rotateZ( float ang ) const
{
	Matrix4x3 newRotation;

	newRotation.m11 = math::cosf( ang );
	newRotation.m12 = math::sinf( ang );

	newRotation.m21 = -math::sinf( ang );
	newRotation.m22 = math::cosf( ang );

	newRotation.m33 = 1;

	return newRotation;
}

//-----------------------------------//

math::Matrix4x3 EulerAngles::getOrientationMatrix() const
{
	return rotateY( yang ) * rotateX( xang ) * rotateZ( zang );
}

//-----------------------------------//

} } // end namespaces