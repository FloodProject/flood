/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/References.h"
#include "Math/BoundingBox.h"
#include "Math/Frustum.h"
#include "Render/Renderable.h"

FWD_DECL_INTRUSIVE(Entity)
FWD_DECL_INTRUSIVE(Renderable)

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

// Builds debug geometry of a bounding box.
RenderablePtr API_ENGINE buildBoundingRenderable( const BoundingBox& box );

// Builds debug geometry of a ray.
EntityPtr API_ENGINE buildRay( const Ray& pickRay, const Vector3& outFar );

// Builds debug geometry of a frustum.
RenderablePtr API_ENGINE buildFrustum( const Frustum& box );

// Updates the debug geometry of a frustum.
void API_ENGINE updateDebugFrustum( const RenderablePtr& rend, const Frustum& box );

//-----------------------------------//

NAMESPACE_ENGINE_END