/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Core/Math/Vector.h"
#include "Core/Math/Ray.h"

NAMESPACE_CORE_BEGIN

//-----------------------------------//

enum struct PlaneIntersection
{
	OnPlane,
	NegativeSide,
	PositiveSide
};

//-----------------------------------//

struct API_CORE Plane
{
	Plane();

	// Build a plane based on normal to plane and a point on the plane.
	Plane( const Vector3& normal, const Vector3& point );

	// Build a plane based on normal to plane and a distance.
	Plane( const Vector3& normal, float distance );

	// Assignment operator.
	Plane& operator = (const Plane& plane);

	// Normalizes the plane.
	void normalize();

	// Gets the distance from a point to the plane.
	float distance(const Vector3& point) const;

	// Gets the distance from a point to the plane.
	float distance(float x, float y, float z) const;

	// Projects the vector on the plane.
	Vector3 project(const Vector3& vec);

	// Returns if point is on the plane.
	//PlaneIntersection::Enum intersects( const Vector3& point ) const;

	// Returns if the plane intersects with bounding box.
	//bool intersects( const BoundingBox& box ) const;

	// Returns if the plane intersects with the ray and also
	// the distance from the ray origin to the plane.
	bool intersects( const Ray& ray, float& distance ) const;

	Vector3 normal;
	float offset;
};

//-----------------------------------//

NAMESPACE_CORE_END
