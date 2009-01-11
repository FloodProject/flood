/************************************************************************
*
* vaporEngine by triton © (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/math/Vector3.h"

namespace vapor {
	namespace math {

//---------------------------------------------------------------------------
// class Matrix4x3
//
// Implement a 4x3 transformation matrix.  This class can represent
// any 3D affine transformation.

class Matrix4x3 {
public:

	// The values of the matrix.  Basically the upper 3x3 portion
	// contains a linear transformation, and the last row is the
	// translation portion.  See the Matrix4x3.cpp for more
	// details.

	float	m11, m12, m13;
	float	m21, m22, m23;
	float	m31, m32, m33;
	float	tx,  ty,  tz;

// Public operations

	// Set to identity

	void	identity();
};

// Operator* is used to transforms a point, and also concatonate matrices.
// The order of multiplications from left to right is the same as
// the order of transformations

Vector3		operator*(const Vector3 &p, const Matrix4x3 &m);
Matrix4x3	operator*(const Matrix4x3 &a, const Matrix4x3 &b);

// Operator *= for conformance to C++ standards

Vector3		&operator*=(Vector3 &p, const Matrix4x3 &m);
Matrix4x3	&operator*=(const Matrix4x3 &a, const Matrix4x3 &m);

// Compute the determinant of the 3x3 portion of the matrix

float	determinant(const Matrix4x3 &m);

// Compute the inverse of a matrix

Matrix4x3 inverse(const Matrix4x3 &m);

} } // end namespaces
