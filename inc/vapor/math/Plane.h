/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/math/Vector3.h"
#include "vapor/math/AABB.h"

namespace vapor {

//-----------------------------------//

namespace PlaneIntersection
{
    enum Enum
    {
        OnPlane,
        NegativeSide,
        PositiveSide
    };
}

class VAPOR_API Plane
{
public:
    // Build a plane based on 3 non-colinear points.
	// Note: Right hand rule will be applied when computing normal.
    Plane( Vector3 const& p1, Vector3 const& p2, Vector3 const& p3 );
    
    // Build a plane based on normal to plane and a point on the plane.
    Plane( Vector3 const& normal, Vector3 const& point );
    
    // Returns if point is on the plane.
	PlaneIntersection::Enum intersect( Vector3 const& point ) const;
    
	// Returns if the plane intersects with bounding box.
    bool intersect( AABB const& aabb ) const;
    
protected:

    Vector3 normal;
    float offset;    
};

//-----------------------------------//

} // end namespace
