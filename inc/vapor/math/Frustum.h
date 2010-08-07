/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/math/Vector2.h"
#include "vapor/math/Matrix4x3.h"
#include "vapor/math/Plane.h"

namespace vapor {

//-----------------------------------//

/**
 * Projection types.
 */

namespace Projection 
{
    enum Enum
    {
        Orthographic,
        Perspective
    };
}

//-----------------------------------//

/**
 * Creates a new Frustum with the field-of-view, aspect ratio, near and far
 * projection settings. It also takes a local to world matrix, because all 
 * of our AABBs are in world space, so when performing object intersection 
 * tests, the frustum also needs to be converted to world space.
 */

class VAPOR_API Frustum
{
public:

    // Constructs the 6 planes making up the frustum, using the passed parameters.
    Frustum();
    Frustum( const Frustum& rhs );

	// Updates the projection matrix.
	void updateProjection( const Vector2i& size );

	// Updates the frustum planes.
	void updatePlanes( const Matrix4x3& matView );

	// Updates the frustum corners.
	void updateCorners( const Matrix4x3& matView );

    // Tests all the points of the AABB to check if they are inside the frustum.
    // Returns true if the aabb is inside or overlaping the frustum, false otherwise
    bool intersect ( const AABB& aabb ) const;

public:

	// Projection type.
	Projection::Enum typeProjection;
	
	// Projection matrix.
	Matrix4x4 matProjection;

	// Field of view.
	float angleFOV;

	// Near clipping plane.
	float nearPlane;
	
	// Far clipping plane.
	float farPlane;

	// Aspect ratio of the frustum.
	float aspectRatio;

    // Stores the planes that make up the frustum.
    // Order: Left, Right, Top, Bottom, Near, Far.
    Plane planes[6];

	// Frustum corner points.
	Vector3 corners[8];
};

//-----------------------------------//

} // end namespace
