/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/math/Matrix4x3.h"
#include "vapor/math/Math.h"

namespace vapor {

//-----------------------------------//

const Matrix4x3 Matrix4x3::Identity;

//-----------------------------------//

Matrix4x3::Matrix4x3() 
{
	identity(); 
}

//-----------------------------------//

void Matrix4x3::identity() 
{
	m11 = 1.0f; m12 = 0.0f; m13 = 0.0f;
	m21 = 0.0f; m22 = 1.0f; m23 = 0.0f;
	m31 = 0.0f; m32 = 0.0f; m33 = 1.0f;
	tx  = 0.0f; ty  = 0.0f; tz  = 0.0f;
}

//-----------------------------------//

Matrix4x3 Matrix4x3::createTranslation( const Vector3& v )
{
	Matrix4x3 s;

	s.tx = v.x;
	s.ty = v.y;
	s.tz = v.z;

	return s;
}

//-----------------------------------//

Matrix4x3 Matrix4x3::createRotation( const EulerAngles& angles )
{
	return rotateX(angles.x)
		 * rotateY(angles.y)
		 * rotateZ(angles.z);
}

//-----------------------------------//

Matrix4x3 Matrix4x3::createFromQuaternion(const Quaternion& q)
{
	// Compute a few values to optimize common subexpressions
	float	ww = 2.0f * q.w;
	float	xx = 2.0f * q.x;
	float	yy = 2.0f * q.y;
	float	zz = 2.0f * q.z;

	// Set the matrix elements.  There is still a little more
	// opportunity for optimization due to the many common
	// subexpressions.  We'll let the compiler handle that...
	Matrix4x3 m;

	m.m11 = 1.0f - yy*q.y - zz*q.z;
	m.m12 = xx*q.y + ww*q.z;
	m.m13 = xx*q.z - ww*q.x;

	m.m21 = xx*q.y - ww*q.z;
	m.m22 = 1.0f - xx*q.x - zz*q.z;
	m.m23 = yy*q.z + ww*q.x;

	m.m31 = xx*q.z + ww*q.y;
	m.m32 = yy*q.z - ww*q.x;
	m.m33 = 1.0f - xx*q.x - yy*q.y;

	return m;
}

//-----------------------------------//

Matrix4x3 Matrix4x3::createScale( const Vector3& v )
{
	Matrix4x3 s;

	s.m11 = v.x;
	s.m22 = v.y;
	s.m33 = v.z;

	return s;
}

//-----------------------------------//

Matrix4x3 Matrix4x3::rotateX( float ang )
{
	const float cos = std::cos( Math::degreeToRadian(ang) );
	const float sin = std::sin( Math::degreeToRadian(ang) );

	Matrix4x3 newRotation;

	newRotation.m11 = 1;

	newRotation.m22 = cos;
	newRotation.m23 = sin;

	newRotation.m32 = -sin;
	newRotation.m33 = cos;

	return newRotation;
}

//-----------------------------------//

Matrix4x3 Matrix4x3::rotateY( float ang )
{
	const float cos = std::cos( Math::degreeToRadian(ang) );
	const float sin = std::sin( Math::degreeToRadian(ang) );

	Matrix4x3 newRotation;

	newRotation.m11 = cos;
	newRotation.m13 = -sin;

	newRotation.m22 = 1;

	newRotation.m31 = sin;
	newRotation.m33 = cos;

	return newRotation;
}

//-----------------------------------//

Matrix4x3 Matrix4x3::rotateZ( float ang )
{
	const float cos = std::cos( Math::degreeToRadian(ang) );
	const float sin = std::sin( Math::degreeToRadian(ang) );

	Matrix4x3 newRotation;

	newRotation.m11 = cos;
	newRotation.m12 = sin;

	newRotation.m21 = -sin;
	newRotation.m22 = cos;

	newRotation.m33 = 1;

	return newRotation;
}

//-----------------------------------//

Vector3	operator*(const Vector3& p, const Matrix4x3& m)
{
	return Vector3(
		p.x*m.m11 + p.y*m.m21 + p.z*m.m31 + m.tx,
		p.x*m.m12 + p.y*m.m22 + p.z*m.m32 + m.ty,
		p.x*m.m13 + p.y*m.m23 + p.z*m.m33 + m.tz
	);
}

//-----------------------------------//

Vector3& operator*=(Vector3& p, const Matrix4x3& m)
{
	p = p * m;
	return p;
}

//-----------------------------------//

Matrix4x3 operator*(const Matrix4x3& a, const Matrix4x3& b) 
{
	Matrix4x3 r;

	// Compute the upper 3x3 (linear transformation) portion
	r.m11 = a.m11*b.m11 + a.m12*b.m21 + a.m13*b.m31;
	r.m12 = a.m11*b.m12 + a.m12*b.m22 + a.m13*b.m32;
	r.m13 = a.m11*b.m13 + a.m12*b.m23 + a.m13*b.m33;

	r.m21 = a.m21*b.m11 + a.m22*b.m21 + a.m23*b.m31;
	r.m22 = a.m21*b.m12 + a.m22*b.m22 + a.m23*b.m32;
	r.m23 = a.m21*b.m13 + a.m22*b.m23 + a.m23*b.m33;

	r.m31 = a.m31*b.m11 + a.m32*b.m21 + a.m33*b.m31;
	r.m32 = a.m31*b.m12 + a.m32*b.m22 + a.m33*b.m32;
	r.m33 = a.m31*b.m13 + a.m32*b.m23 + a.m33*b.m33;

	// Compute the translation portion
	r.tx = a.tx*b.m11 + a.ty*b.m21 + a.tz*b.m31 + b.tx;
	r.ty = a.tx*b.m12 + a.ty*b.m22 + a.tz*b.m32 + b.ty;
	r.tz = a.tx*b.m13 + a.ty*b.m23 + a.tz*b.m33 + b.tz;

	return r;
}

//-----------------------------------//

Matrix4x4 operator*(const Matrix4x3& a, const Matrix4x4& b) 
{
	return Matrix4x4(a)*b;
}

//-----------------------------------//

Matrix4x3& operator*=(Matrix4x3& a, const Matrix4x3& b) 
{
	a = a * b;
	return a;
}

//-----------------------------------//

float determinant(const Matrix4x3& m)
{
	return
		  m.m11 * (m.m22*m.m33 - m.m23*m.m32)
		+ m.m12 * (m.m23*m.m31 - m.m21*m.m33)
		+ m.m13 * (m.m21*m.m32 - m.m22*m.m31);
}

//-----------------------------------//

Matrix4x3 inverse(const Matrix4x3& m) {

	// Compute the determinant

	float det = determinant(m);

	// If we're singular, then the determinant is zero and there's
	// no inverse

	//assert(fabs(det) > 0.000001f);

	// Compute one over the determinant, so we divide once and
	// can *multiply* per element

	float	oneOverDet = 1.0f / det;

	// Compute the 3x3 portion of the inverse, by
	// dividing the adjoint by the determinant

	Matrix4x3	r;

	r.m11 = (m.m22*m.m33 - m.m23*m.m32) * oneOverDet;
	r.m12 = (m.m13*m.m32 - m.m12*m.m33) * oneOverDet;
	r.m13 = (m.m12*m.m23 - m.m13*m.m22) * oneOverDet;

	r.m21 = (m.m23*m.m31 - m.m21*m.m33) * oneOverDet;
	r.m22 = (m.m11*m.m33 - m.m13*m.m31) * oneOverDet;
	r.m23 = (m.m13*m.m21 - m.m11*m.m23) * oneOverDet;

	r.m31 = (m.m21*m.m32 - m.m22*m.m31) * oneOverDet;
	r.m32 = (m.m12*m.m31 - m.m11*m.m32) * oneOverDet;
	r.m33 = (m.m11*m.m22 - m.m12*m.m21) * oneOverDet;

	// Compute the translation portion of the inverse

	r.tx = -(m.tx*r.m11 + m.ty*r.m21 + m.tz*r.m31);
	r.ty = -(m.tx*r.m12 + m.ty*r.m22 + m.tz*r.m32);
	r.tz = -(m.tx*r.m13 + m.ty*r.m23 + m.tz*r.m33);

	return r;
}

//-----------------------------------//

} // end namespace
