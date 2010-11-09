/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "math/Vector3.h"
#include "math/Vector4.h"

namespace vapor {

class Matrix4x3;

//-----------------------------------//

/**
 * Implements a 4x4 transformation matrix that can represent the full 
 * range of types of operations that can be used in computer graphics.
 * Usually you just want to use the 4x3 because it suffices for the most
 * common operations, like translation, rotation and scaling, but in the
 * case of projections a full 4x4 matrix is needed.
 */

class VAPOR_API Matrix4x4
{
public:

	Matrix4x4();
	Matrix4x4(const Matrix4x3&);

	// Sets this to the identity matrix.
	void identity();

	// Returns the inverse of the matrix.
	Matrix4x4 inverse() const;

	// Returns the inverse of the matrix.
	Matrix4x4 transpose() const;

	// Multiplies two matrices together.
	Matrix4x4 operator*(const Matrix4x4& m) const;

	// Multiplies two matrices together.
	Matrix4x4 operator*(const Matrix4x3& m) const;

	// Multiplies a vector by the matrix.
	Vector3 operator*( const Vector3& v ) const;

	// Multiplies a vector by the matrix.
	Vector4 operator*( const Vector4& v ) const;

	// Gets a projection matrix from a set of parameters.
	static Matrix4x4 createPerspectiveProjection( float FOV,
		float aspectRatio, float nearPlane, float farPlane );

	// Gets an orthographic matrix from a set of parameters.
	static Matrix4x4 createOrthographicProjection( float left, float right,
		float bottom, float top, float nearPlane, float farPlane );

	// Identity matrix.
	static const Matrix4x4 Identity;

public:

	float	m11, m12, m13, m14;
	float	m21, m22, m23, m24;
	float	m31, m32, m33, m34;
	float	tx,  ty,  tz,  tw;
};

//-----------------------------------//

} // end namespace
