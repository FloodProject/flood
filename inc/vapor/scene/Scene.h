/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "scene/Group.h"
#include "render/Renderable.h"
#include "math/Matrix4x3.h"
#include "math/Ray.h"
#include "math/Frustum.h"

namespace vapor {

//-----------------------------------//

struct RayQueryResult
{
	EntityPtr entity;
	float distance;
};

typedef std::vector<RayQueryResult> RayQueryList;

//-----------------------------------//

struct RayTriangleQueryResult : public RayQueryResult
{
	GeometryPtr geometry;
	RenderablePtr renderable;
	Vector3 intersection;
	Vector3 intersectionUV;
	Vector3 trianglePosition[3];
	Vector3 triangleUV[3];
};

//-----------------------------------//

/**
 * A scene instance holds all the entities that are attached to it.
 * You might have different scenes for different 'levels' of your game
 * and switch between them in runtime. The scene is also responsible
 * for updating all the game entities and will aswell call hooks like 
 * before performing frustum culling or before rendering the entity.
 */

class VAPOR_API Scene : public Group
{
	DECLARE_CLASS_()

public:

	Scene();

	// Updates all the entities recursively.
	virtual void update( double delta );

	// Checks for collision via ray-box tests.
	bool doRayBoxQuery( const Ray& ray, RayQueryResult& res );
	bool doRayBoxQuery( const Ray& ray, RayQueryList& list, bool all = true );
	
	// Checks for collision via frustum-box tests.
	bool doRayVolumeQuery( const Frustum& volume, RayQueryList& list, bool all = true );

	// Checks for collision via ray-triangle tests.
	bool doRayTriangleQuery( const Ray& ray, RayTriangleQueryResult& res );
	bool doRayTriangleQuery( const Ray& ray, RayTriangleQueryResult& res, const EntityPtr& );

	// // Checks for collision via ray-renderable tests.
	bool doRayRendQuery( const Ray&, const RenderablePtr&, RayTriangleQueryResult& );
};

TYPEDEF_SHARED_POINTER_FROM_TYPE( Scene );
TYPEDEF_SHARED_WEAK_POINTER_FROM_TYPE( Scene );

//-----------------------------------//

} // end namespace