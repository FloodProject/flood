/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/math/Matrix4x4.h"
#include "vapor/math/Matrix4x3.h"
#include "vapor/math/Math.h"

namespace vapor {

const VAPOR_API Matrix4x4 Matrix4x4::Identity;

//-----------------------------------//

Matrix4x4::Matrix4x4()
{
	identity(); 
}

//-----------------------------------//

Matrix4x4::Matrix4x4(const Matrix4x3& m)
{
	m11 = m.m11; m12 = m.m12; m13 = m.m13; m14 = 0.0;
	m21 = m.m21; m22 = m.m22; m23 = m.m23; m24 = 0.0;
	m31 = m.m31; m32 = m.m32; m33 = m.m33; m34 = 0.0;
	tx  = m.tx;  ty  = m.ty;  tz  = m.tz;  tw  = 1.0;
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

Matrix4x4 Matrix4x4::operator *(const Matrix4x4& m2) const
{
    Matrix4x4 r;
    r.m11 = m11 * m2.m11 + m12 * m2.m21 + m13 * m2.m31 + m14 * m2.tx;
    r.m12 = m11 * m2.m12 + m12 * m2.m22 + m13 * m2.m32 + m14 * m2.ty;
    r.m13 = m11 * m2.m13 + m12 * m2.m23 + m13 * m2.m33 + m14 * m2.tz;
    r.m14 = m11 * m2.m14 + m12 * m2.m24 + m13 * m2.m34 + m14 * m2.tw;

    r.m21 = m21 * m2.m11 + m22 * m2.m21 + m23 * m2.m31 + m24 * m2.tx;
    r.m22 = m21 * m2.m12 + m22 * m2.m22 + m23 * m2.m32 + m24 * m2.ty;
    r.m23 = m21 * m2.m13 + m22 * m2.m23 + m23 * m2.m33 + m24 * m2.tz;
    r.m24 = m21 * m2.m14 + m22 * m2.m24 + m23 * m2.m34 + m24 * m2.tw;

    r.m31 = m31 * m2.m11 + m32 * m2.m21 + m33 * m2.m31 + m34 * m2.tx;
    r.m32 = m31 * m2.m12 + m32 * m2.m22 + m33 * m2.m32 + m34 * m2.ty;
    r.m33 = m31 * m2.m13 + m32 * m2.m23 + m33 * m2.m33 + m34 * m2.tz;
    r.m34 = m31 * m2.m14 + m32 * m2.m24 + m33 * m2.m34 + m34 * m2.tw;

    r.tx = tx * m2.m11 + ty * m2.m21 + tz * m2.m31 + tw * m2.tx;
    r.ty = tx * m2.m12 + ty * m2.m22 + tz * m2.m32 + tw * m2.ty;
    r.tz = tx * m2.m13 + ty * m2.m23 + tz * m2.m33 + tw * m2.tz;
    r.tw = tx * m2.m14 + ty * m2.m24 + tz * m2.m34 + tw * m2.tw;

    return r;
}

//-----------------------------------//

Vector3 Matrix4x4::operator *( const Vector3& v ) const
{
    Vector3 r;

	r.x = v.x * m11 + v.y * m21 + v.z * m31 + tx;
	r.y = v.x * m12 + v.y * m22 + v.z * m32 + ty;
	r.z = v.x * m13 + v.y * m23 + v.z * m33 + tz;

    return r;
}

//-----------------------------------//

Vector4 Matrix4x4::operator *( const Vector4& v ) const
{
    Vector4 r;

	r.x = v.x * m11 + v.y * m21 + v.z * m31 + v.w * tx;
	r.y = v.x * m12 + v.y * m22 + v.z * m32 + v.w * ty;
	r.z = v.x * m13 + v.y * m23 + v.z * m33 + v.w * tz;
	r.w = v.x * m14 + v.y * m24 + v.z * m34 + v.w * tw;

    return r;
}

//-----------------------------------//

Matrix4x4 Matrix4x4::inverse() const
{
    int colIdx[] = { 0, 0, 0, 0 };
    int rowIdx[] = { 0, 0, 0, 0 };
    int pivotIdx[] = { -1, -1, -1, -1 };

    // convert the matrix to an array for easy looping
	float inverse[4][4] = {
		{ m11, m12, m13, m14 },
		{ m21, m22, m23, m24 },
		{ m31, m32, m33, m34 },
		{ tx,  ty,  tz,  tw  }
	};

    int icol = 0;
    int irow = 0;
    for (int i = 0; i < 4; i++)
    {
        // Find the largest pivot value
        float maxPivot = 0.0f;
        for (int j = 0; j < 4; j++)
        {
            if (pivotIdx[j] != 0)
            {
                for (int k = 0; k < 4; ++k)
                {
                    if (pivotIdx[k] == -1)
                    {
						float absVal = ::abs(inverse[j][k]);
                        if (absVal > maxPivot)
                        {
                            maxPivot = absVal;
                            irow = j;
                            icol = k;
                        }
                    }
                    else if (pivotIdx[k] > 0)
                    {
                        return *this;
                    }
                }
            }
        }

        ++(pivotIdx[icol]);

        // Swap rows over so pivot is on diagonal
        if (irow != icol)
        {
            for (int k = 0; k < 4; ++k)
            {
                float f = inverse[irow][k];
                inverse[irow][k] = inverse[icol][k];
                inverse[icol][k] = f;
            }
        }

        rowIdx[i] = irow;
        colIdx[i] = icol;

        float pivot = inverse[icol][icol];
        // check for singular matrix
        if (pivot == 0.0f)
        {
			warn("math", "Matrix is singular and cannot be inverted.");
            return *this;
        }
  
	    // Scale row so it has a unit diagonal
        float oneOverPivot = 1.0f / pivot;
        inverse[icol][icol] = 1.0f;
        for (int k = 0; k < 4; ++k)
            inverse[icol][k] *= oneOverPivot;

        // Do elimination of non-diagonal elements
        for (int j = 0; j < 4; ++j)
        {
            // check this isn't on the diagonal
            if (icol != j)
            {
                float f = inverse[j][icol];
                inverse[j][icol] = 0.0f;
                for (int k = 0; k < 4; ++k)
                    inverse[j][k] -= inverse[icol][k] * f;
            }
        }
    }

    for (int j = 3; j >= 0; --j)
    {
        int ir = rowIdx[j];
        int ic = colIdx[j];
        for (int k = 0; k < 4; ++k)
        {
            float f = inverse[k][ir];
            inverse[k][ir] = inverse[k][ic];
            inverse[k][ic] = f;
        }
    }

	Matrix4x4 mat;
	memcpy(&mat.m11, inverse, sizeof(inverse));

	return mat;
}

//-----------------------------------//

Matrix4x4 Matrix4x4::createPerspectiveProjection( float fov, float ar, float near_, float far_ ) 
{
	assert( near_ > 0 );
	assert( far_  > 0 );
	assert( near_ < far_ );

	Matrix4x4 proj;

	float h = 1.0f / tanf( Math::degreeToRadian(fov) * 0.5f );
	float neg_depth = (near_ - far_);
	
	proj.m11 = h / ar;
	proj.m12 = 0;
	proj.m13 = 0;
	proj.m14 = 0;
	
	proj.m21 = 0;
	proj.m22 = h;
	proj.m23 = 0;
	proj.m24 = 0;

	proj.m31 = 0;
	proj.m32 = 0;
	proj.m33 = (far_ + near_) / neg_depth;
	proj.m34 = -1.0;

	proj.tx = 0;
	proj.ty = 0;
	proj.tz = 2.0f * (near_* far_) / neg_depth;
	proj.tw = 0;

	return proj;
}

//-----------------------------------//

Matrix4x4 Matrix4x4::createOrthographicProjection( float left, float right,
		float bottom, float top, float near_, float far_)
{
	Matrix4x4 proj;

	proj.m11 = 2.0f / (right - left);
	proj.m12 = 0;
	proj.m13 = 0;
	proj.m14 = 0;
	
	proj.m21 = 0;
	proj.m22 = 2.0f / (top - bottom);
	proj.m23 = 0;
	proj.m24 = 0;

	proj.m31 = 0;
	proj.m32 = 0;
	proj.m33 = -2.0f / (far_ - near_);
	proj.m34 = 0;

	proj.tx = -(right + left) / (right - left);
	proj.ty = -(top + bottom) / (top - bottom);
	proj.tz = -(far_ + near_) / (far_ - near_);
	proj.tw = 1;

	return proj;	
}

//-----------------------------------//

} // end namespace
