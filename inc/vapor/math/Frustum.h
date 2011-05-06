/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/Reflection.h"
#include "Math/Vector.h"
#include "Math/Matrix4x4.h"
#include "Math/Plane.h"
#include "Math/BoundingBox.h"

NAMESPACE_EXTERN_BEGIN

//-----------------------------------//

/**
 * Projection types.
 */

enum_class Projection
{
	Orthographic,
	Perspective  
};

API_CORE REFLECT_DECLARE_ENUM(Projection);

//-----------------------------------//

/**
 * Frustum is a shape similiar to a pyramid with the top truncated.
 */

struct API_CORE Frustum // : public Object
{
    Frustum();
    Frustum( const Frustum& rhs );

	// Updates the projection matrix.
	void updateProjection( const Vector2& size );

	// Updates the frustum planes.
	void updatePlanes( const Matrix4x3& matView );

	// Updates the frustum corners.
	void updateCorners( const Matrix4x3& matView );

    // Checks if the box is inside the frustum.
    bool intersects( const BoundingBox& box ) const;

public:

	// Projection type.
	Projection projection;
	
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

API_CORE REFLECT_DECLARE_CLASS(Frustum)

//-----------------------------------//

NAMESPACE_EXTERN_END
