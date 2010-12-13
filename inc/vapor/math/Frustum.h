/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "math/Vector2.h"
#include "math/Matrix4x4.h"
#include "math/Plane.h"
#include "math/BoundingBox.h"

#include "core/Enum.h"
#include "core/Class.h"

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

	DECLARE_ENUM()
}

//-----------------------------------//

/**
 * Frustum is a shape similiar to a pyramid with the top truncated.
 */

class CORE_API Frustum
{
	DECLARE_CLASS_()

public:

    Frustum();
    Frustum( const Frustum& rhs );

	// Updates the projection matrix.
	void updateProjection( const Vector2i& size );

	// Updates the frustum planes.
	void updatePlanes( const Matrix4x3& matView );

	// Updates the frustum corners.
	void updateCorners( const Matrix4x3& matView );

    // Checks if the box is inside the frustum.
    bool intersects( const BoundingBox& box ) const;

public:

	// Projection type.
	Projection::Enum projection;
	
	// Projection matrix.
	Matrix4x4 matProjection;

	// Field of view.
	float fieldOfView;

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
