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
#include "Math/Color.h"

FWD_DECL_INTRUSIVE(Entity)
FWD_DECL_INTRUSIVE(Renderable)
FWD_DECL_INTRUSIVE(VertexBuffer)

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

/**
 * Debug drawing is used to draw debugging representations of objects
 * by components and scripts. To make this easier to the user, the
 * rendering of objects is done in an immediate-mode like fashion,
 * with the management of the vertex buffers and materials done by
 * the engine. Picking of objects is also supported.
 */

class API_ENGINE DebugDrawer
{
public:

	DebugDrawer();
	~DebugDrawer();

	// Sets the color for the debug draw.
	void setColor( const Color& color );

	// Draws a debug bounding box.
	void drawBox( const BoundingBox& box );

	// Draws a debug ray.
	void drawRay( const Ray& ray, float length );

	// Draws a debug frustum.
	void drawFrustum( const Frustum& frustum );

	// Draws a debug icon.
	void drawIcon( const Vector3& pos );

	// Cleans up all the stored geometry.
	void reset();

	RenderablePtr lines;
	RenderablePtr triangles;
	RenderablePtr quads;

	std::vector<RenderablePtr> renderables;
};

namespace DebugDrawFlags
{
	enum Enum
	{
		Selected = (1 << 0)
	};
};

class GeometryBuffer;

// Builds debug geometry of a bounding box.
API_ENGINE RenderablePtr DebugBuildBoundingBox( const BoundingBox& box );

// Updates the debug geometry of a bounding box.
API_ENGINE void DebugUpdateBoudingBox( GeometryBuffer*, const BoundingBox& box );

// Builds debug geometry of a ray.
API_ENGINE RenderablePtr DebugBuildRay( const Ray& pickRay, float length );

// Builds debug geometry of a frustum.
API_ENGINE RenderablePtr DebugBuildFrustum( const Frustum& box );

// Updates the debug geometry of a frustum.
API_ENGINE void DebugUpdateFrustum( const RenderablePtr&, const Frustum& box );

//-----------------------------------//

NAMESPACE_ENGINE_END