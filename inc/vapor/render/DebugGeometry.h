/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/math/AABB.h"

FWD_DECL_NS_TYPEDEF_INT(render, Renderable)
FWD_DECL_NS_TYPEDEF_SHARED(scene, Node)

namespace vapor { namespace render {

//-----------------------------------//

RenderablePtr VAPOR_API buildBoundingRenderable( const math::AABB& aabb );
scene::NodePtr VAPOR_API buildRay( const math::Ray& pickRay, const math::Vector3& outFar );

//-----------------------------------//

} } // end namespaces