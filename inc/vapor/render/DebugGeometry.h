/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/math/AABB.h"
#include "vapor/render/Renderable.h"

namespace vapor { namespace render {

//-----------------------------------//

RenderablePtr buildBoundingRenderable( const math::AABB& aabb );

//-----------------------------------//

} } // end namespaces