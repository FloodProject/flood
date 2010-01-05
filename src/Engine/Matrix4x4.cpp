/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#include "vapor/math/Matrix4x4.h"
#include "vapor/math/Math.h"

namespace vapor { namespace math {

//-----------------------------------//

const Matrix4x4 Matrix4x4::Identity;

//-----------------------------------//

Matrix4x4::Matrix4x4()
{
	identity(); 
}

//-----------------------------------//

void Matrix4x4::identity() 
{
	m11 = 1.0f; m12 = 0.0f; m13 = 0.0f; m14 = 0.0;
	m21 = 0.0f; m22 = 1.0f; m23 = 0.0f; m24 = 0.0;
	m31 = 0.0f; m32 = 0.0f; m33 = 1.0f; m34 = 0.0;
	tx  = 0.0f; ty  = 0.0f; tz  = 0.0f; tw  = 1.0;
}

//-----------------------------------//

Matrix4x4 Matrix4x4::createPerspectiveProjection( float fov, float ar, float near_, float far_ ) 
{
	Matrix4x4 projectionMatrix;

	float h = 1.0f / math::tanf( math::degreeToRadian( fov ) / 2.0f );
	float neg_depth = (near_ - far_);
	
	projectionMatrix.m11 = h / ar;
	projectionMatrix.m12 = 0;
	projectionMatrix.m13 = 0;
	projectionMatrix.m14 = 0;
	
	projectionMatrix.m21 = 0;
	projectionMatrix.m22 = h;
	projectionMatrix.m23 = 0;
	projectionMatrix.m24 = 0;

	projectionMatrix.m31 = 0;
	projectionMatrix.m32 = 0;
	projectionMatrix.m33 = (far_ + near_) / neg_depth;
	projectionMatrix.m34 = 2.0f * (near_* far_) / neg_depth;

	projectionMatrix.tx = 0;
	projectionMatrix.ty = 0;
	projectionMatrix.tz = -1.0;
	projectionMatrix.tw = 0;

	return projectionMatrix;
}

//-----------------------------------//

Matrix4x4 Matrix4x4::createOrthographicProjection( float left, float right, float bottom, float top,
		float near_, float far_)
{
	Matrix4x4 projectionMatrix;

	projectionMatrix.m11 = 2.0f / (right - left);
	projectionMatrix.m12 = 0;
	projectionMatrix.m13 = 0;
	projectionMatrix.m14 = 0;
	
	projectionMatrix.m21 = 0;
	projectionMatrix.m22 = 2.0f / (top - bottom) ;
	projectionMatrix.m23 = 0;
	projectionMatrix.m24 = 0;

	projectionMatrix.m31 = 0;
	projectionMatrix.m32 = 0;
	projectionMatrix.m33 = -2.0f / (far_ - near_);
	projectionMatrix.m34 = 0;

	projectionMatrix.tx = -(right + left) / (right - left);
	projectionMatrix.ty = -(top + bottom) / (top - bottom);
	projectionMatrix.tz = -(far_ + near_) / (far_ - near_);
	projectionMatrix.tw = 1;

	return projectionMatrix;	
}

//-----------------------------------//

} } // end namespaces
