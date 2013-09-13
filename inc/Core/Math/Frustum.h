/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Core/API.h"
#include "Core/Reflection.h"
#include "Core/Object.h"
#include "Core/Math/Vector.h"
#include "Core/Math/Matrix4x4.h"
#include "Core/Math/Plane.h"
#include "Core/Math/BoundingBox.h"

NAMESPACE_CORE_BEGIN

//-----------------------------------//

/**
 * Projection types.
 */

enum struct FrustumProjection
{
	Orthographic,
	Perspective
};

API_CORE REFLECT_DECLARE_ENUM(Projection);

//-----------------------------------//

/**
 * Frustum is a shape similiar to a pyramid with the top truncated.
 */

API_CORE REFLECT_DECLARE_CLASS(Frustum)

struct API_CORE FLD_VALUE_TYPE Frustum
{
	//REFLECT_DECLARE_OBJECT(Frustum)

	Frustum();
	Frustum( const Frustum& rhs );

	// Updates the projection matrix.
	void updateProjection();

	// Updates the frustum planes.
	void updatePlanes( const Matrix4x3& matView );

	// Updates the frustum corners.
	void updateCorners( const Matrix4x3& matView );

	// Checks if the box is inside the frustum.
	bool intersects( const BoundingBox& box ) const;

public:

	// Projection type.
	FrustumProjection projection;

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

	// Ortographic view size.
	Vector3 orthoSize;

	// Stores the planes that make up the frustum.
	// Order: Left, Right, Top, Bottom, Near, Far.
	Plane planes[6];

	// Frustum corner points.
	Vector3 corners[8];
};

//-----------------------------------//

NAMESPACE_CORE_END