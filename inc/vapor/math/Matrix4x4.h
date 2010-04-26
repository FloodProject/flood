/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/math/Matrix4x3.h"

namespace vapor { namespace math {

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

	// Multiplies two matrices together.
	Matrix4x4 operator *(const Matrix4x4& m2) const;

	// Multiplies a vector by the matrix.
	Vector3 operator *( const Vector3 &v ) const;

	// Gets a projection matrix from a set of parameters.
	static Matrix4x4 createPerspectiveProjection( float fov, float ar,
		float near_, float far_ );

	// Gets an orthographic matrix from a set of parameters.
	static Matrix4x4 createOrthographicProjection( float left, float right,
		float bottom, float top,
		float near_, float far_ );

	// Identity matrix.
	static const Matrix4x4 Identity;

public:

	float	m11, m12, m13, m14;
	float	m21, m22, m23, m24;
	float	m31, m32, m33, m34;
	float	tx,  ty,  tz,  tw;
};

//-----------------------------------//

} } // end namespaces
