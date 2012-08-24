/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"
#include "Scene/Scene.h"
#include "Scene/Transform.h"
#include "Scene/Tags.h"
#include "Scene/Geometry.h"
#include "Scene/SceneLoader.h"
#include "Graphics/RenderDevice.h"

#include "Core/Profiler.h"
#include "Core/Serialization.h"
#include "Core/SerializationHelpers.h"

#include "Scene/Model.h"
#include "Resources/Animation.h"
#include "Resources/Skeleton.h"

#include <jansson.h>

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

void SerializeScene(ReflectionContext* context, ReflectionWalkType::Enum wt)
{
	if( !context->loading )
	{
		ReflectionWalkComposite(context);
		return;
	}
	
	SerializerJSON* json = (SerializerJSON*) context->userData;
	json_t* top = json->values.back();

	json_t* val = json_object_get(top, "entities");

	if( val )
	{
		context->walkCompositeFields(context, wt);
	}
	else
	{
		const Field* field = context->field;
		context->field = ClassGetField(context->composite, "entities");

		json->values.push_back(top);
		context->walkCompositeField(context, wt);
		json->values.pop_back();

		context->field = field;
	}
}

REFLECT_CHILD_CLASS(Scene, Resource)
	REFLECT_CLASS_SET_SERIALIZER(SerializeScene)
	FIELD_CLASS(10, Group, entities)
REFLECT_CLASS_END()

//-----------------------------------//

Scene::Scene()
{
	SceneLoaderGetType();
	entities.addReference();
}

//-----------------------------------//

Scene::~Scene()
{
	LogDebug("Destroying Scene");
}

//-----------------------------------//

struct Culler
{
	Culler()
		: ray(nullptr)
		, frustum(nullptr)
	{}
	
	const Ray* ray;
	const Frustum* frustum;
};

static bool sortRayQueryResult(const RayQueryResult& lhs, const RayQueryResult& rhs)
{
	return lhs.distance < rhs.distance;
}

static bool doRayGroupQuery( const Group* group, const Culler& culler, RayQueryList& list, bool all )
{
	const std::vector<EntityPtr>& entities = group->getEntities();

	// Do some ray casting to find a collision.
	for( size_t i = 0; i < entities.size(); i++ )
	{
		Entity* entity = entities[i].get();
		if( !entity ) continue;

		Class* klass = entity->getType();

		if( ClassInherits(klass, ReflectionGetType(Group)) )
		{
			const Group* group = (const Group*) entity;

			if( doRayGroupQuery(group, culler, list, all) && !all )
				return true;
		}
		else
		{
			// Ignore invisible entities.
			if( !entity->isVisible() || entity->getTag(Tags::NonPickable) )
				continue;

			const Transform* transform = entity->getTransform().get();
			if( !transform ) continue;

			const BoundingBox& box = transform->getWorldBoundingVolume();

			float distance = -1;

			bool intersectsBox = culler.ray && box.intersects(*culler.ray, distance);
			bool intersectsFrustum = culler.frustum && culler.frustum->intersects(box);
			
			if(intersectsBox || intersectsFrustum)
			{
				RayQueryResult res;
				res.entity = entity;
				res.distance = distance;

				list.push_back( res );

				if( !all ) break;
			}
		}
	}

	// Sort the results by distance.
	std::sort( list.begin(), list.end(), &sortRayQueryResult );

	return !list.empty();
}

//-----------------------------------//

bool Scene::doRayBoxQuery( const Ray& ray, RayQueryResult& res )
{
	RayQueryList list;
	
	if( !doRayBoxQuery(ray, list, false) )
		return false;

	res = list.front();	
	return true;
}

//-----------------------------------//

bool Scene::doRayBoxQuery( const Ray& ray, RayQueryList& list, bool all )
{
	Culler culler;
	culler.ray = &ray;

	return doRayGroupQuery(&entities, culler, list, all);
}

//-----------------------------------//

bool Scene::doRayVolumeQuery( const Frustum& volume, RayQueryList& list, bool all )
{
	Culler culler;
	culler.frustum = &volume;

	return doRayGroupQuery(&entities, culler, list, all);
}

//-----------------------------------//

bool Scene::doRayTriangleQuery( const Ray& ray, RayTriangleQueryResult& res )
{
	// Perform ray casting to find the entities.
	RayQueryList list;
	doRayBoxQuery( ray, list );

	for( size_t i = 0; i < list.size(); i++ )
	{
		const RayQueryResult& query = list[i];
	
		if( doRayTriangleQuery( ray, res, query.entity ) )
			return true;
	}

	return false;
}

//-----------------------------------//

static void BuildQueryResult( RayTriangleQueryResult& res, const Ray& ray,
	const Vector3(&pos)[3], const Vector3(&tex)[3], float u, float v, float t)
{
	res.trianglePosition[0] = pos[0];
	res.trianglePosition[1] = pos[1];
	res.trianglePosition[2] = pos[2];

	res.triangleUV[0] = tex[0];
	res.triangleUV[1] = tex[1];
	res.triangleUV[2] = tex[2];

	res.intersectionLocal = ray.getPoint(t);

	res.intersectionUV.x = u;
	res.intersectionUV.y = v;
}

//-----------------------------------//

static bool DoRayQueryIndexed( const Ray& ray, const GeometryBuffer* gb,
	Vector3* verticesVec, uint32 numVertices, int8 vertexStride,
	RayTriangleQueryResult& res )
{
	int32 indexSizeBytes = gb->indexSize / 8;
	
	uint16* indices = (uint16*) &gb->indexData.front();
	uint32 numIndices = gb->indexData.size() / indexSizeBytes;

	uint8* texCoords = (uint8*) gb->getAttribute(VertexAttribute::TexCoord0, 0);
	size_t texCoordsOffset = gb->declarations.getOffset(VertexAttribute::TexCoord0);

	uint8* vertices = (uint8*) verticesVec;

	for( size_t i = 0; i < numIndices; i += 3 )
	{
		#pragma TODO("Validate the index is not out-of-bounds")

		uint16 i0 = indices[i+0];
		uint16 i1 = indices[i+1];
		uint16 i2 = indices[i+2];

		Vector3 tri[3];
		tri[0] = *(Vector3*) gb->getAttribute(VertexAttribute::Position, i0);
		tri[1] = *(Vector3*) gb->getAttribute(VertexAttribute::Position, i1);
		tri[2] = *(Vector3*) gb->getAttribute(VertexAttribute::Position, i2);

		float t, u, v;
		float &o = u, &n = v;
		
		if( !ray.intersects(tri,t,o,n) )
			continue;

		Vector3 tex[3];

		if( texCoords )
		{
			tex[0] = *(Vector2*) gb->getAttribute(VertexAttribute::TexCoord0, i0);
			tex[1] = *(Vector2*) gb->getAttribute(VertexAttribute::TexCoord0, i1);
			tex[2] = *(Vector2*) gb->getAttribute(VertexAttribute::TexCoord0, i2);
		}

		BuildQueryResult(res, ray, tri, tex, u, v, t);
		return true;
	}

	return false;
}

//-----------------------------------//

static bool DoRayQuery( const Ray& ray,	const GeometryBuffer* gb,
	Vector3* vertices, uint32 numVertices, int8 vertexStride,
	RayTriangleQueryResult& res )
{
	Vector3* texCoords = (Vector3*) gb->getAttribute(VertexAttribute::TexCoord0, 0);
	
	for( size_t i = 0; i < numVertices; i += 3 )
	{
		const Vector3(&tri)[3] = (const Vector3(&)[3]) vertices[i + vertexStride];

		float t, u, v;
		float &o = u, &n = v;

		if( !ray.intersects(tri,t,o,n) )
			continue;

		const Vector3 (&tex)[3] = (const Vector3 (&)[3]) texCoords[i];

		BuildQueryResult(res, ray, tri, tex, u, v, t);
		return true;
	}
	
	return false;
}

//-----------------------------------//

static bool DoSkinning(const Geometry* geo, std::vector<Vector3>& skinnedPositions)
{
	bool isModel = ClassInherits(geo->getType(), ReflectionGetType(Model));
	if( !isModel ) return false;

	Model* model = (Model*) geo;
	
	Mesh* mesh = model->getMesh().Resolve();
	if( !mesh || !mesh->isAnimated() ) return false;

	// Resize the output vector to be of the proper size.
	size_t size = mesh->getGeometryBuffer()->getNumVertices();

	skinnedPositions.resize(size);

	if( model->isHardwareSkinned() )
	{
		model->doSkinningRaw(&skinnedPositions[0]);
		return true;
	}

	return false;
}

//-----------------------------------//

static Ray TransformRay(const Ray& ray, const EntityPtr& entity)
{
	const TransformPtr& transform = entity->getTransform();
	Matrix4x3 absolute = transform->getAbsoluteTransform().inverse();

	Ray transRay;
	transRay.origin = absolute * ray.origin;

	absolute.tx = 0;
	absolute.ty = 0;
	absolute.tz = 0;

	transRay.direction = (absolute * ray.direction).normalize();

	return transRay;
}

//-----------------------------------//

bool Scene::doRayTriangleQuery( const Ray& ray, RayTriangleQueryResult& res, const EntityPtr& entity )
{
	Ray entityRay = TransformRay(ray, entity);
	const std::vector<GeometryPtr>& geoms = entity->getGeometry();
	
	const Transform* transform = entity->getTransform().get();
	Matrix4x3 absolute = transform->getAbsoluteTransform();

	for( size_t i = 0; i < geoms.size(); i++ )
	{
		const Geometry* geo = geoms[i].get();
		if( !geo ) continue;

		// Let's do a rough bounding volume level intersection test on each
		// individual geometry of the node. This helps cut the number of 
		// collisions tests in nodes with lots of geometry components.

		const BoundingBox& bound = geo->getWorldBoundingVolume();

		float distance;
		
		if( !bound.intersects(ray, distance) )
			continue;

		const std::vector<RenderBatchPtr>& rends = geo->getRenderables();

		std::vector<Vector3> skinnedPositions;
		bool didSkinning = DoSkinning(geo, skinnedPositions);

		for( size_t j = 0; j < rends.size(); j++ )
		{
			const RenderBatch* rend = rends[j].get();
	
			if( rend->getPrimitiveType() != PrimitiveType::Triangles )
				continue;

			const GeometryBuffer* gb = rend->getGeometryBuffer().get();

			Vector3* vertices = (Vector3*) gb->getAttribute(VertexAttribute::Position, 0);
			uint32 numVertices = gb->getNumVertices();

			int8 vertexStride = gb->getAttributeStride(VertexAttribute::Position);
			
			if(didSkinning)
			{
				vertices = (Vector3*) &skinnedPositions.front();
				vertexStride = 0;
			}

			bool isIndexed = gb->isIndexed();

			bool hit = false;

			if( !isIndexed )
				hit = DoRayQuery(entityRay, gb, vertices, numVertices, vertexStride, res);
			else
				hit = DoRayQueryIndexed(entityRay, gb, vertices, numVertices, vertexStride, res);

			if(hit)
			{
				res.intersectionWorld = absolute * res.intersectionLocal;
				res.entity = entity.get();
				res.geometry = (Geometry*) geo;
				res.renderable = (Renderable*) rend;
				res.distance = (ray.origin - res.intersectionWorld).lengthSquared();

				return true;
			}
		}
	}

	return false;
}

//-----------------------------------//

void Scene::update( float delta )
{
	entities.update( delta );
}

//-----------------------------------//

NAMESPACE_ENGINE_END