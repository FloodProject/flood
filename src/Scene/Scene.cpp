/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "Scene/Scene.h"
#include "Scene/Transform.h"
#include "Scene/Tags.h"
#include "Scene/Geometry.h"
#include <algorithm>

namespace vapor {

//-----------------------------------//

BEGIN_CLASS_PARENT(Scene, Group)
END_CLASS()

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
	for( uint i = 0; i < entities.size(); i++ )
	{
		const EntityPtr& entity = entities[i];
		const Type& type = entity->getType();

		if( type.is<Group>() || type.inherits<Group>() )
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

			if((culler.ray && box.intersects(*culler.ray, distance)) ||
			   (culler.frustum && culler.frustum->intersects(box)))
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

	for( uint i = 0; i < list.size(); i++ )
	{
		const RayQueryResult& query = list[i];
	
		if( doRayTriangleQuery( ray, res, query.entity ) )
			return true;
	}

	return false;
}

//-----------------------------------//

bool Scene::doRayTriangleQuery( const Ray& ray, RayTriangleQueryResult& res,
							    const EntityPtr& node )
{
	const std::vector<GeometryPtr>& geoms = node->getGeometry();

	const TransformPtr& transform = node->getTransform();
	Matrix4x3 absolute = transform->getAbsoluteTransform().inverse();

	Ray transRay;
	transRay.origin = absolute * ray.origin;

	absolute.tx = 0;
	absolute.ty = 0;
	absolute.tz = 0;

	transRay.direction = (absolute * ray.direction).normalize();

	// Down to triangle picking.	
	for( uint i = 0; i < geoms.size(); i++ )
	{		
		const GeometryPtr& geo = geoms[i];
	
		if( !geo )
			continue;

		// Let's do a rough bounding volume level intersection test on each
		// individual geometry of the node. This helps cut the number of 
		// collisions tests in nodes with lots of geometry components.

		const BoundingBox& bound = geo->getWorldBoundingVolume();

		float distance;
		
		if( !bound.intersects(ray, distance) )
			continue;

		const std::vector<RenderablePtr>& rends = geo->getRenderables();

		for( uint j = 0; j < rends.size(); j++ )
		{
			const RenderablePtr& rend = rends[j];

			if( doRayRendQuery(transRay, rend, res) )
			{
				res.entity = node;
				res.geometry = geo;
				return true;
			}
		}
	}

	return false;
}

//-----------------------------------//

bool Scene::doRayRendQuery( const Ray& ray, const RenderablePtr& rend, RayTriangleQueryResult& res )
{
	if( !rend ) return false;
	if( rend->getPrimitiveType() != PolygonType::Triangles ) return false;

	const VertexBufferPtr& vb = rend->getVertexBuffer();
	const IndexBufferPtr& ib = rend->getIndexBuffer();
	
	if( !vb ) return false;

	const std::vector<Vector3>& texCoords = vb->getAttribute(VertexAttribute::TexCoord0);
	const std::vector<Vector3>& vertices = vb->getAttribute(VertexAttribute::Position);
				
	uint size = ib ? ib->getSize() : vertices.size();
	
	int indexSizeBytes = ib->indexSize / 8;
	
	for( uint i = 0; i < size; i += 3 )
	{
		int index = ib ? *(short int*) &ib->data[indexSizeBytes * i] : i;

		const Vector3 (&tri)[3] = (const Vector3 (&)[3]) vertices[index];		
		float t, o, n;
		
		if( !ray.intersects(tri,t,o,n) )
			continue;

		// Ray intersected, build results.

		res.trianglePosition[0] = tri[0];
		res.trianglePosition[1] = tri[1];
		res.trianglePosition[2] = tri[2];

		const Vector3 (&uv)[3] = (const Vector3 (&)[3]) texCoords[index];

		res.triangleUV[0] = uv[0];
		res.triangleUV[1] = uv[1];
		res.triangleUV[2] = uv[2];

		res.intersectionUV.x = o;
		res.intersectionUV.y = n;
		res.intersection = ray.getPoint(t);
		res.distance = t;
		res.renderable = rend;

		return true;
	}

	return false;
}

//-----------------------------------//

void Scene::update( float delta )
{
	Group::update( delta );
}

//-----------------------------------//

} // end namespace