/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Resources/Resource.h"
#include "Engine/Scene/Group.h"
#include "Graphics/RenderBatch.h"
#include "Core/Math/Matrix4x3.h"
#include "Core/Math/Ray.h"
#include "Core/Math/Frustum.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

FWD_DECL_INTRUSIVE(Entity)

struct API_SCENE RayQueryResult
{
	Entity* entity;
	float distance;
};

typedef std::vector<RayQueryResult> RayQueryList;

//-----------------------------------//

FWD_DECL_INTRUSIVE(Geometry)

struct API_SCENE RayTriangleQueryResult : public RayQueryResult
{
	Geometry* geometry;
	RenderBatch* renderable;

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

API_SCENE REFLECT_DECLARE_CLASS(Scene)

class API_SCENE Scene : public Resource
{
	REFLECT_DECLARE_OBJECT(Scene);

public:

	Scene();
	virtual ~Scene();

	// Return the proper resource group for this resource.
	GETTER(ResourceGroup, ResourceGroup, ResourceGroup::Scenes)

	// Updates all the scene groups recursively.
	void update( float delta );

	// Checks for collision via ray-box tests.
	bool doRayBoxQuery( const Ray& ray, RayQueryResult& res );
	bool doRayBoxQuery( const Ray& ray, RayQueryList& list, bool all = true );
	
	// Checks for collision via frustum-box tests.
	bool doRayVolumeQuery( const Frustum& volume, RayQueryList& list, bool all = true );

	// Checks for collision via ray-triangle tests.
	bool doRayTriangleQuery( const Ray& ray, RayTriangleQueryResult& res );
	bool doRayTriangleQuery( const Ray& ray, RayTriangleQueryResult& res, const EntityPtr& );

	// Entities of the scene.
	FLD_READONLY Group entities;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Scene );
TYPEDEF_RESOURCE_HANDLE_FROM_TYPE( Scene );

//-----------------------------------//

NAMESPACE_ENGINE_END