/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "math/BoundingBox.h"
#include "math/Frustum.h"
#include "ReferenceCount.h"

FWD_DECL_SHARED(Entity)
FWD_DECL_INTRUSIVE(Renderable)

namespace vapor {

//-----------------------------------//

// Builds debug geometry of a bounding box.
RenderablePtr VAPOR_API buildBoundingRenderable( const BoundingBox& box );

// Builds debug geometry of a ray.
EntityPtr VAPOR_API buildRay( const Ray& pickRay, const Vector3& outFar );

// Builds debug geometry of a frustum.
RenderablePtr VAPOR_API buildFrustum( const Frustum& box );

// Updates the debug geometry of a frustum.
void VAPOR_API updateFrustum( const RenderablePtr& rend, const Frustum& box );

//-----------------------------------//

} // end namespace