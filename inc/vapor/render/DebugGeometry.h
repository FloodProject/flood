/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/math/AABB.h"

FWD_DECL_TYPEDEF_INT(Renderable)
FWD_DECL_TYPEDEF_SHARED(Node)

namespace vapor {

//-----------------------------------//

RenderablePtr VAPOR_API buildBoundingRenderable( const AABB& aabb );
NodePtr VAPOR_API buildRay( const Ray& pickRay, const Vector3& outFar );

//-----------------------------------//

} // end namespace