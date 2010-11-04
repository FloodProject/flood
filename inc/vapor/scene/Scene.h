/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/scene/Group.h"
#include "vapor/math/Matrix4x3.h"
#include "vapor/math/Ray.h"

namespace vapor {

//-----------------------------------//

// Bounding-box based intersection
struct RayBoxQueryResult
{
	NodePtr node;
	float distance;
};

// Triangle based intersection
struct RayTriangleQueryResult
{
	GeometryPtr geometry;
	RenderablePtr renderable;
	Vector3 intersection;
	Vector3 intersectionUV;
	Vector3 trianglePosition[3];
	Vector3 triangleUV[3];
	float distance;
};

typedef std::vector<RayBoxQueryResult> RayBoxQueryList;

//-----------------------------------//

/**
 * Scene tree that holds all the nodes attached to the scene. This is the
 * main interface that the engine programmer will use to attach/detach all
 * different types of content to the scene. There are different types of 
 * nodes that can be used in the scene, like Mesh, Light, Camera, Particle,
 * Listener, Trigger, Billboard and maybe other that we want to later add.
 */

class VAPOR_API Scene : public Group
{
	DECLARE_CLASS_()

public:

	Scene();
	virtual ~Scene();

	// Updates all the entities recursively.
	virtual void update( double delta );

	// Checks for collision via ray-BoundingBox tests.
	bool doRayBoxQuery( const Ray& ray, RayBoxQueryResult& res );
	bool doRayBoxQuery( const Ray& ray, RayBoxQueryList& list, bool all = true );
	
	// Checks for collision via ray-triangle tests.
	bool doRayTriangleQuery( const Ray& ray, RayTriangleQueryResult& res );
	bool doRayTriangleQuery( const Ray& ray, RayTriangleQueryResult& res, const NodePtr& node );
};

TYPEDEF_SHARED_POINTER_FROM_TYPE( Scene );
TYPEDEF_SHARED_WEAK_POINTER_FROM_TYPE( Scene );

//-----------------------------------//

} // end namespace