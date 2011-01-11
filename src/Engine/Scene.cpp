/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "scene/Scene.h"
#include "scene/Transform.h"
#include "scene/Tags.h"
#include "scene/Geometry.h"
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

static bool doRayGroupQuery( const GroupPtr& group, const Culler& culler, RayQueryList& list, bool all )
{
	const std::vector<EntityPtr>& entities = group->getEntities();

	// Do some ray casting to find a collision.
	for( uint i = 0; i < entities.size(); i++ )
	{
		const EntityPtr& entity = entities[i];
		const Type& type = entity->getType();

		if( type.is<Group>() || type.inherits<Group>() )
		{
			GroupPtr group = std::static_pointer_cast<Group>(entity);

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
	GroupPtr group = std::static_pointer_cast<Group>(shared_from_this());

	Culler culler;
	culler.ray = &ray;

	return doRayGroupQuery(group, culler, list, all);
}

//-----------------------------------//

bool Scene::doRayVolumeQuery( const Frustum& volume, RayQueryList& list, bool all )
{
	GroupPtr group = std::static_pointer_cast<Group>(shared_from_this());

	Culler culler;
	culler.frustum = &volume;

	return doRayGroupQuery(group, culler, list, all);
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
	if( !rend )
		return false;

	if( rend->getPrimitiveType() != PolygonType::Triangles )
		return false;

	const VertexBufferPtr& vb = rend->getVertexBuffer();
			
	if( !vb )
		return false;

	const std::vector<Vector3>& UVs = vb->getAttribute(VertexAttribute::TexCoord0);

	const std::vector<Vector3>& vertices = vb->getVertices();
	uint size = vertices.size();
			
	const IndexBufferPtr& ib = rend->getIndexBuffer();
	std::vector<ushort> indices;

	if( ib )
	{
		indices = ib->getIndices16();
		size = indices.size();
	}

	for( uint i = 0; i < size; i += 3 )
	{
		Vector3 tri[3];
		tri[0] = ib ? vertices[indices[i+0]] : vertices[i+0];
		tri[1] = ib ? vertices[indices[i+1]] : vertices[i+1];
		tri[2] = ib ? vertices[indices[i+2]] : vertices[i+2];

		Vector3 uv[3];
		uv[0] = ib ? UVs[indices[i+0]] : UVs[i+0];
		uv[1] = ib ? UVs[indices[i+1]] : UVs[i+1];
		uv[2] = ib ? UVs[indices[i+2]] : UVs[i+2];

		float t; float u, v;
		if( ray.intersects(tri, t, u, v) )
		{
			for( uint i = 0; i < 3; i++ )
				res.trianglePosition[i] = tri[i];

			for( uint i = 0; i < 3; i++ )
				res.triangleUV[i] = uv[i];

			res.intersectionUV.x = u;
			res.intersectionUV.y = v;
			res.intersection = ray.getPoint(t);
			res.distance = t;
			res.renderable = rend;

			return true;
		}
	}

	return false;
}

//-----------------------------------//

void Scene::update( double delta )
{
	Group::update( delta );
}

//-----------------------------------//

} // end namespace