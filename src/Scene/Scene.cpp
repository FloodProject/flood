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
#include "Scene/Model.h"
#include "Render/Device.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(Scene, Group)
REFLECT_CLASS_END()

//-----------------------------------//

Scene::Scene() : Group("Scene")
{ }

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
		const EntityPtr& entity = entities[i];
		if( !entity ) continue;

		Class* klass = entity->getType();

		if( ClassInherits(klass, ReflectionGetType(Group)) )
		{
			const Group* group = (const Group*) entity.get();

			if( doRayGroupQuery(group, culler, list, all) && !all )
				return true;
		}
		else
		{
			// Ignore invisible entities.
			if( !entity->isVisible() || entity->getTag(Tags::NonPickable) )
				continue;

			const TransformPtr& transform = entity->getTransform();
			
			if( !transform )
				continue;

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

	return doRayGroupQuery(this, culler, list, all);
}

//-----------------------------------//

bool Scene::doRayVolumeQuery( const Frustum& volume, RayQueryList& list, bool all )
{
	Culler culler;
	culler.frustum = &volume;

	return doRayGroupQuery(this, culler, list, all);
}

//-----------------------------------//

bool Scene::doRayTriangleQuery( const Ray& ray, RayTriangleQueryResult& res )
{
	// Perform ray casting to find the nodes.
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

static void buildResult( RayTriangleQueryResult& res, const Ray& ray,
	const Vector3(&pos)[3], const Vector3(&tex)[3], float u, float v, float t)
{
	res.trianglePosition[0] = pos[0];
	res.trianglePosition[1] = pos[1];
	res.trianglePosition[1] = pos[2];

	res.triangleUV[0] = tex[0];
	res.triangleUV[1] = tex[1];
	res.triangleUV[2] = tex[2];

	res.intersectionLocal = ray.getPoint(t);
	res.intersectionWorld = 

	res.intersectionUV.x = u;
	res.intersectionUV.y = v;

	res.distance = t;
}

//-----------------------------------//

#define index(i) (*(uint16*) &ib->data[indexSizeBytes * (i)])

static bool doRayQueryIndexed( const Ray& ray, const IndexBufferPtr& ib,
	const std::vector<Vector3>& vertices, const std::vector<Vector3>& texCoords,
	RayTriangleQueryResult& res )
{				
	size_t size = ib->getSize();
	int32 indexSizeBytes = ib->indexSize / 8;
	
	for( size_t i = 0; i < size; i += 3 )
	{
		Vector3 tri[3];
		tri[0] = vertices[index(i+0)];
		tri[1] = vertices[index(i+1)];
		tri[2] = vertices[index(i+2)];

		float t, o, n;
		if( !ray.intersects(tri,t,o,n) )
			continue;

		Vector3 tex[3];
		tex[0] = texCoords[index(i+0)];
		tex[1] = texCoords[index(i+1)];
		tex[2] = texCoords[index(i+2)];

		buildResult(res, ray, tri, tex, o, n, t);
		return true;
	}

	return false;
}

//-----------------------------------//

static bool doRayQuery( const Ray& ray,	const std::vector<Vector3>& vertices,
	const std::vector<Vector3>& texCoords, RayTriangleQueryResult& res )
{				
	for( size_t i = 0; i < vertices.size(); i += 3 )
	{
		const Vector3 (&tri)[3] = (const Vector3 (&)[3]) vertices[i];		

		float t, o, n;
		if( !ray.intersects(tri,t,o,n) )
			continue;

		const Vector3 (&tex)[3] = (const Vector3 (&)[3]) texCoords[i];

		buildResult(res, ray, tri, tex, o, n, t);
		return true;
	}

	return false;
}

//-----------------------------------//

static bool needsSkinning(const GeometryPtr& geo, std::vector<Vector3>& skinnedPositions)
{
	bool isModel = ClassInherits(geo->getType(), ReflectionGetType(Model));
	
	if( !isModel )
		return false;

	ModelPtr model = RefCast<Model>(geo);
	
	Mesh* mesh = model->getMesh().Resolve();
	if( !mesh || !mesh->isAnimated() ) return false;

	bool isHardwareSkinned = model->isHardwareSkinned();
	if( isHardwareSkinned ) model->doSkinning(skinnedPositions);

	return isHardwareSkinned;
}

//-----------------------------------//

static Ray doTransformRay(const Ray& ray, const EntityPtr& entity)
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
	Ray transRay = doTransformRay(ray, entity);
	const std::vector<GeometryPtr>& geoms = entity->getGeometry();
	
	const TransformPtr& transform = entity->getTransform();
	Matrix4x3 absolute = transform->getAbsoluteTransform();

	for( size_t i = 0; i < geoms.size(); i++ )
	{		
		const GeometryPtr& geo = geoms[i];
		if( !geo ) continue;

		// Let's do a rough bounding volume level intersection test on each
		// individual geometry of the node. This helps cut the number of 
		// collisions tests in nodes with lots of geometry components.

		const BoundingBox& bound = geo->getWorldBoundingVolume();

		float distance;
		
		if( !bound.intersects(ray, distance) )
			continue;

		const std::vector<RenderablePtr>& rends = geo->getRenderables();

		std::vector<Vector3> skinnedPositions;
		bool doSkinning = needsSkinning(geo, skinnedPositions);

		for( size_t j = 0; j < rends.size(); j++ )
		{
			const RenderablePtr& rend = rends[j];
	
			if( rend->getPrimitiveType() != PolygonType::Triangles )
				continue;

			const VertexBufferPtr& vb = rend->getVertexBuffer();
			const IndexBufferPtr& ib = rend->getIndexBuffer();

			if( !vb ) return false;

			const std::vector<Vector3>& vertices = vb->getAttribute(VertexAttribute::Position);
			const std::vector<Vector3>& finalVertices = doSkinning ? skinnedPositions : vertices;
			const std::vector<Vector3>& texCoords = vb->getAttribute(VertexAttribute::TexCoord0);
			
			bool isIndexed = ib != nullptr;

			bool hit = false;

			if( !isIndexed )
				hit = doRayQuery(transRay, finalVertices, texCoords, res);
			else
				hit = doRayQueryIndexed(transRay, ib, finalVertices, texCoords, res);

			if(hit)
			{
				res.intersectionWorld = absolute * res.intersectionLocal;
				res.entity = entity;
				res.geometry = geo;
				res.renderable = rend;

				return true;
			}
		}
	}

	return false;
}

//-----------------------------------//

void Scene::update( float delta )
{
	Group::update( delta );
}

//-----------------------------------//

NAMESPACE_ENGINE_END