/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "math/Vector3.h"
#include "math/EulerAngles.h"
#include "math/Quaternion.h"

namespace vapor {

//-----------------------------------//

/**
 * Implements a 4x3 transformation matrix.  This class can represent
 * any 3D affine transformation, like scaling, translation, rotation,
 * shearing, etc.
 */

class VAPOR_API Matrix4x3 
{
public:

	Matrix4x3();

	// Set to identity matrix.
	void identity();

	// Compute the determinant of the 3x3 portion of the matrix.
	float determinant() const;

	// Compute the inverse of a matrix.
	Matrix4x3 inverse() const;

	// Transforms the point by the matrix.
	Vector3 operator*(const Vector3& p) const;
	
	// Concatenates the matrices.
	Matrix4x3 operator*(const Matrix4x3& m) const;

	// Operator *= for conformance to C++ standards.
	//Vector3& operator*=(Vector3 &p);
	//Matrix4x3& operator*=(Matrix4x3& m);

	// Linear interpolation between two matrices.
	static Matrix4x3 lerp(const Matrix4x3& a, const Matrix4x3& b, float t);

	// Creates a new translation matrix from a translation vector.
	static Matrix4x3 createTranslation( const Vector3& translate );

	// Creates a new orientation matrix from an Euler angles vector.
	static Matrix4x3 createRotation( const EulerAngles& angles );

	// Creates a new scaling matrix from a scale vector.
	static Matrix4x3 createScale( const Vector3& scale );

	// Creates a new orientation matrix from a quaternion.
	static Matrix4x3 createFromQuaternion(const Quaternion& q);

	// Creates a rotation matrix by the X, Y or Z axis.
	static Matrix4x3 rotateX( float ang );
	static Matrix4x3 rotateY( float ang );
	static Matrix4x3 rotateZ( float ang );

public:

	// The values of the matrix. Basically the upper 3x3 portion
	// contains a linear transformation, and the last row is the
	// translation portion.

	float	m11, m12, m13;
	float	m21, m22, m23;
	float	m31, m32, m33;
	float	tx,  ty,  tz;

	// Special matrices
	static const Matrix4x3 Identity;
};

//-----------------------------------//

} // end namespace
