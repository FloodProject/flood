/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/math/Matrix.h"
#include "vapor/math/Plane.h"

namespace vapor {
	namespace math {

//-----------------------------------//

/**
 * Creates a new Frustum with the field-of-view, aspect ratio, near and far
 * projection settings. It also takes a local to world matrix, because all 
 * of our AABBs are in world space, so when performing object intersection 
 * tests, the frustum also needs to be converted to world space.
 */

class Frustum
{
public:

    // Constructs the 6 planes making up the frustum, using the passed parameters.
    Frustum ( float FOV, float AspectRatio, float near, float far, Matrix4 localToWorld);
    
    // Tests all the points of the AABB to check if they are inside the frustum.
    // Returns true if the aabb is inside or overlaping the frustum, false otherwise
    bool intersect ( const AABB& aabb ) const;

private:
    
    // Stores the planes that make up the frustum like this:
    //     near, far, left, right, top, bottom
    Plane planes[6];
};

//-----------------------------------//

} } // end namespaces
