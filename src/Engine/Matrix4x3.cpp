/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"

#include "vapor/math/Matrix4x3.h"
#include "vapor/math/Vector3.h"

/////////////////////////////////////////////////////////////////////////////
//
// Notes:
//
// See Chapter 11 for more information on class design decisions.
//
//---------------------------------------------------------------------------
//
// MATRIX ORGANIZATION
//
// The purpose of this class is so that a user might perform transformations
// without fiddling with plus or minus signs or transposing the matrix
// until the output "looks right."  But of course, the specifics of the
// internal representation is important.  Not only for the implementation
// in this file to be correct, but occasionally direct access to the
// matrix variables is necessary, or beneficial for optimization.  Thus,
// we document our matrix conventions here.
//
// We use row vectors, so multiplying by our matrix looks like this:
//
//               | m11 m12 m13 |
//     [ x y z ] | m21 m22 m23 | = [ x' y' z' ]
//               | m31 m32 m33 |
//               | tx  ty  tz  |
//
// Strict adherence to linear algebra rules dictates that this
// multiplication is actually undefined.  To circumvent this, we can
// consider the input and output vectors as having an assumed fourth
// coordinate of 1.  Also, since we cannot technically invert a 4x3 matrix
// according to linear algebra rules, we will also assume a rightmost
// column of [ 0 0 0 1 ].  This is shown below:
//
//                 | m11 m12 m13 0 |
//     [ x y z 1 ] | m21 m22 m23 0 | = [ x' y' z' 1 ]
//                 | m31 m32 m33 0 |
//                 | tx  ty  tz  1 |
//
// In case you have forgotten your linear algebra rules for multiplying
// matrices (which are described in section 7.1.6 and 7.1.7), see the
// definition of operator* for the expanded computations.
//
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
// Matrix4x3 class members
//
/////////////////////////////////////////////////////////////////////////////

namespace vapor { namespace math {

//-----------------------------------//

Matrix4x3::Matrix4x3() 
{
	identity(); 
}

//-----------------------------------//

//---------------------------------------------------------------------------
// Matrix4x3::identity
//
// Set the matrix to identity

void Matrix4x3::identity() 
{
	m11 = 1.0f; m12 = 0.0f; m13 = 0.0f;
	m21 = 0.0f; m22 = 1.0f; m23 = 0.0f;
	m31 = 0.0f; m32 = 0.0f; m33 = 1.0f;
	tx  = 0.0f; ty  = 0.0f; tz  = 0.0f;
}

//-----------------------------------//

Matrix4x3 Matrix4x3::createScaleMatrix( const Vector3& v )
{
	Matrix4x3 s;

	s.m11 = v.x;
	s.m22 = v.y;
	s.m33 = v.z;

	return s;
}

//-----------------------------------//

Matrix4x3 Matrix4x3::createTranslationMatrix( const Vector3& v )
{
	Matrix4x3 s;

	s.tx = v.x;
	s.ty = v.y;
	s.tz = v.z;

	return s;
}

//---------------------------------------------------------------------------
// Vector * Matrix4x3
//
// Transform the point.  This makes using the vector class look like it
// does with linear algebra notation on paper.
//
// We also provide a *= operator, as per C convention.
//
// See 7.1.7

Vector3	operator*(const Vector3 &p, const Matrix4x3 &m)
{
	return Vector3(
		p.x*m.m11 + p.y*m.m21 + p.z*m.m31 + m.tx,
		p.x*m.m12 + p.y*m.m22 + p.z*m.m32 + m.ty,
		p.x*m.m13 + p.y*m.m23 + p.z*m.m33 + m.tz
	);
}

//-----------------------------------//

Vector3 &operator*=(Vector3 &p, const Matrix4x3 &m)
{
	p = p * m;
	return p;
}

//---------------------------------------------------------------------------
// Matrix4x3 * Matrix4x3
//
// Matrix concatenation.  This makes using the vector class look like it
// does with linear algebra notation on paper.
//
// We also provide a *= operator, as per C convention.
//
// See 7.1.6

Matrix4x3 operator*(const Matrix4x3 &a, const Matrix4x3 &b) 
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

	// Return it.  Ouch - involves a copy constructor call.  If speed
	// is critical, we may need a seperate function which places the
	// result where we want it...

	return r;
}

//-----------------------------------//

Matrix4x3 &operator*=(Matrix4x3 &a, const Matrix4x3 &b) 
{
	a = a * b;
	return a;
}

//---------------------------------------------------------------------------
// determinant
//
// Compute the determinant of the 3x3 portion of the matrix.
//
// See 9.1.1 for more info.

float determinant(const Matrix4x3 &m)
{
	return
		  m.m11 * (m.m22*m.m33 - m.m23*m.m32)
		+ m.m12 * (m.m23*m.m31 - m.m21*m.m33)
		+ m.m13 * (m.m21*m.m32 - m.m22*m.m31);
}

//---------------------------------------------------------------------------
// inverse
//
// Compute the inverse of a matrix.  We use the classical adjoint divided
// by the determinant method.
//
// See 9.2.1 for more info.

Matrix4x3 inverse(const Matrix4x3 &m) {

	// Compute the determinant

	float	det = determinant(m);

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

	// Return it.  Ouch - involves a copy constructor call.  If speed
	// is critical, we may need a separate function which places the
	// result where we want it...

	return r;
}

//-----------------------------------//

const Matrix4x3 Matrix4x3::Identity;

//-----------------------------------//

} } // end namespaces
