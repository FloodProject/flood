/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "math/Vector3.h"
#include "math/BoundingBox.h"
#include "math/Ray.h"

namespace vapor {

//-----------------------------------//

struct PlaneIntersection
{
    enum Enum
    {
        OnPlane,
        NegativeSide,
        PositiveSide
    };
};

//-----------------------------------//

class VAPOR_API Plane
{
public:

    // Build a plane based on 3 non-colinear points.
	// Note: Right hand rule will be applied when computing normal.
	Plane();
    Plane( const Vector3& p1, const Vector3& p2, const Vector3& p3 );
    Plane( const Vector3& normal, float d );

    // Build a plane based on normal to plane and a point on the plane.
    //Plane( const Vector3& normal, const Vector3& point );
    
	// Normalizes the plane.
	void normalize();

    // Returns if point is on the plane.
	//PlaneIntersection::Enum intersects( const Vector3& point ) const;
    
	// Returns if the plane intersects with bounding box.
    //bool intersects( const BoundingBox& box ) const;

    // Returns distance from ray origin to plane.
	bool intersects( const Ray& ray, float& distance ) const;

public:

    Vector3 normal;
    float offset;
};

//-----------------------------------//

} // end namespace
