/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Engine/API.h"
#include "Core/References.h"

FWD_DECL_INTRUSIVE(RenderBatch)

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

enum class FLD_FLAGS DebugDrawFlags : uint8
{
	Selected = (1 << 0)
};

struct BoundingBox;
struct Frustum;
struct Ray;

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

	Color currentColor;

	RenderBatchPtr lines;
	RenderBatchPtr triangles;
	RenderBatchPtr quads;

	std::vector<RenderBatch*> renderables;
};

class GeometryBuffer;

// Builds debug geometry of a bounding box.
FLD_IGNORE API_ENGINE RenderBatchPtr DebugBuildBoundingBox( const BoundingBox& box );

// Updates the debug geometry of a bounding box.
FLD_IGNORE API_ENGINE void DebugUpdateBoudingBox( GeometryBuffer*, const BoundingBox&, Color );

// Builds debug geometry of a ray.
FLD_IGNORE API_ENGINE RenderBatchPtr DebugBuildRay( const Ray& pickRay, float length );

// Builds debug geometry of a frustum.
FLD_IGNORE API_ENGINE RenderBatchPtr DebugBuildFrustum( const Frustum& box );

// Updates the debug geometry of a frustum.
FLD_IGNORE API_ENGINE void DebugUpdateFrustum( const RenderBatchPtr&, const Frustum& box );

//-----------------------------------//

NAMESPACE_ENGINE_END
