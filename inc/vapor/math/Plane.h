/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/math/Vector3.h"
#include "vapor/math/AABB.h"

namespace vapor {
	namespace math {

//-----------------------------------//

/**
 * Nice plane.
 */

class VAPOR_API Plane
{
public:

    // build a plane based on 3 non colinear points.  Right hand rule will be applied when computing normal
    // to plane formed by p1, p2 ,p3
    Plane( Vector3 const& p1, Vector3 const& p2, Vector3 const& p3 );
    
    // build plane based on normal to plane and a point on the plane
    Plane( Vector3 const& normal, Vector3 const& point );

    enum Status
    {
        OnPlane,
        NegativeSide,
        PositiveSide
    };
    
    // returns if point is on the plane, on the positive side or negative side of plane
    Status intersect( Vector3 const& point ) const;
    
    bool intersect( AABB const& aabb ) const;
    
private:

    Vector3 normal;
    float offset;    
};

//-----------------------------------//

} } // end namespaces
