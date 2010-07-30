/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/math/Vector3.h"
#include "vapor/math/EulerAngles.h"
#include "vapor/math/Matrix4x4.h"

namespace vapor {

//-----------------------------------//

/**
 * Implement a 4x3 transformation matrix.  This class can represent
 * any 3D affine transformation, like scaling, translation, rotation,
 * shearing, etc.
 */

class VAPOR_API Matrix4x3 
{
public:

	Matrix4x3();

	// Set to identity
	void identity();

	// Creates a new translation matrix from a given translation vector.
	static Matrix4x3 createTranslation( const Vector3& translate );

	// Creates a new orientation matrix from a given angles vector.
	static Matrix4x3 createOrientation( const EulerAngles& angles );

	// Creates a new scaling matrix from a given scale vector.
	static Matrix4x3 createScale( const Vector3& scale );

	static Matrix4x3 rotateX( float ang );
	static Matrix4x3 rotateY( float ang );
	static Matrix4x3 rotateZ( float ang );

public:

	// The values of the matrix.  Basically the upper 3x3 portion
	// contains a linear transformation, and the last row is the
	// translation portion.  See the Matrix4x3.cpp for more
	// details.

	float	m11, m12, m13;
	float	m21, m22, m23;
	float	m31, m32, m33;
	float	tx,  ty,  tz;

	// Special matrices

	static const Matrix4x3 Identity;
};

//-----------------------------------//

// Operator* is used to transforms a point, and also concatonate matrices.
// The order of multiplications from left to right is the same as
// the order of transformations

Vector3		VAPOR_API operator*(const Vector3 &p, const Matrix4x3 &m);
Matrix4x3	VAPOR_API operator*(const Matrix4x3 &a, const Matrix4x3 &b);
Matrix4x4	VAPOR_API operator*(const Matrix4x3 &a, const Matrix4x4 &b);

// Operator *= for conformance to C++ standards

VAPOR_API Vector3& operator*=(Vector3 &p, const Matrix4x3 &m);
VAPOR_API Matrix4x3& operator*=(Matrix4x3& a, const Matrix4x3& b);

// Compute the determinant of the 3x3 portion of the matrix

float	VAPOR_API determinant(const Matrix4x3 &m);

// Compute the inverse of a matrix

Matrix4x3 VAPOR_API inverse(const Matrix4x3 &m);

//-----------------------------------//

} // end namespace
