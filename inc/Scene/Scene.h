/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Scene/Group.h"
#include "Render/Renderable.h"
#include "Math/Matrix4x3.h"
#include "Math/Ray.h"
#include "Math/Frustum.h"

NAMESPACE_ENGINE_BEGIN

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

	Vector3 intersectionLocal;
	Vector3 intersectionWorld;
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

REFLECT_DECLARE_CLASS(Scene)

class API_SCENE Scene : public Group
{
	REFLECT_DECLARE_OBJECT(Scene);

public:

	Scene();

	// Updates all the entities recursively.
	virtual void update( float delta );

	// Checks for collision via ray-box tests.
	bool doRayBoxQuery( const Ray& ray, RayQueryResult& res );
	bool doRayBoxQuery( const Ray& ray, RayQueryList& list, bool all = true );
	
	// Checks for collision via frustum-box tests.
	bool doRayVolumeQuery( const Frustum& volume, RayQueryList& list, bool all = true );

	// Checks for collision via ray-triangle tests.
	bool doRayTriangleQuery( const Ray& ray, RayTriangleQueryResult& res );
	bool doRayTriangleQuery( const Ray& ray, RayTriangleQueryResult& res, const EntityPtr& );
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Scene );

//-----------------------------------//

NAMESPACE_ENGINE_END