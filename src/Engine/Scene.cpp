/************************************************************************
*
* vapor3D Engine � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "scene/Scene.h"
#include "scene/Transform.h"
#include "scene/Tags.h"
#include "scene/Geometry.h"

namespace vapor {

//-----------------------------------//

BEGIN_CLASS_PARENT(Scene, Group)
END_CLASS()

//-----------------------------------//

Scene::Scene()
	: Group("Scene")
{ }

//-----------------------------------//

Scene::~Scene()
{ }

//-----------------------------------//

bool Scene::doRayBoxQuery( const Ray& ray, RayBoxQueryResult& res )
{
	RayBoxQueryList list;
	
	if( !doRayBoxQuery(ray, list) )
		return false;

	res = list.front();	
	return true;
}

//-----------------------------------//

static bool sortRayBoxQueryResult(const RayBoxQueryResult& lhs, const RayBoxQueryResult& rhs)
{
	return lhs.distance < rhs.distance;
}

static bool doRayBoxGroupQuery( const GroupPtr& group, const Ray& ray,
							   RayBoxQueryList& list, bool all )
{
	// Do some ray casting to find a collision.
	for( uint i = 0; i < group->getEntities().size(); i++ )
	{
		const EntityPtr& entity = group->getEntities()[i];
		const Type& type = entity->getInstanceType();

		if( type.is<Group>() || type.inherits<Group>() )
		{
			GroupPtr group = std::static_pointer_cast<Group>(entity);

			if( doRayBoxGroupQuery(group, ray, list, all) && !all )
				return true;
		}
		else
		{
			// No need to pick invisible nodes.
			if( !entity->isVisible() || entity->getTag(Tags::NonPickable) )
				continue;

			const TransformPtr& transform = entity->getTransform();
			
			if( !transform )
				continue;

			const BoundingBox& box = transform->getWorldBoundingVolume();
				
			float distance;
			if( box.intersects(ray, distance) )
			{
				RayBoxQueryResult res;
				res.node = entity;
				res.distance = distance;

				list.push_back( res );

				if( !all ) break;
			}
		}
	}

	// Sort the results by distance.
	std::sort( list.begin(), list.end(), &sortRayBoxQueryResult );

	return !list.empty();
}

bool Scene::doRayBoxQuery( const Ray& ray, RayBoxQueryList& list, bool all )
{
	GroupPtr group = std::static_pointer_cast<Group>(shared_from_this());
	return doRayBoxGroupQuery(group, ray, list, all);
}

//-----------------------------------//

bool Scene::doRayTriangleQuery( const Ray& ray, RayTriangleQueryResult& res )
{
	// Perform ray casting to find the nodes.
	RayBoxQueryList list;
	doRayBoxQuery( ray, list );

	for( uint i = 0; i < list.size(); i++ )
	{
		const RayBoxQueryResult& query = list[i];
	
		if( doRayTriangleQuery( ray, res, query.node ) )
			return true;
	}

	return false;
}

//-----------------------------------//

bool Scene::doRayTriangleQuery( const Ray& ray, RayTriangleQueryResult& res,
							    const EntityPtr& node )
{
	// Down to triangle picking.	
	for( uint i = 0; i < node->getGeometry().size(); i++ )
	{		
		const GeometryPtr& geo = node->getGeometry()[i];
	
		if( !geo )
			continue;

		// Let's do a rough bounding volume level intersection test on each
		// individual geometry of the node. This helps cut the number of 
		// collisions tests in nodes with lots of geometry components.

		const BoundingBox& bound = geo->getBoundingVolume();

		float distance;
		
		if( !bound.intersects(ray, distance) )
			continue;

		for( uint e = 0; e < geo->getRenderables().size(); e++ )
		{
			const RenderablePtr& rend = geo->getRenderables()[e];

			// This picking method only works on triangles.

			if( !rend )
				continue;

			if( rend->getPrimitiveType() != PolygonType::Triangles )
				continue;

			const VertexBufferPtr& vb = rend->getVertexBuffer();
			
			if( !vb )
				continue;

			const std::vector<Vector3>& UVs = 
				vb->getAttribute(VertexAttribute::TexCoord0);

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

				Vector3 to; float n; float u, v;
				if( ray.intersects(tri, to, n, u, v) )
				{					
					for( byte i = 0; i < 3; i++ )
						res.trianglePosition[i] = tri[i];

					for( byte i = 0; i < 3; i++ )
						res.triangleUV[i] = uv[i];

					res.intersectionUV.x = u;
					res.intersectionUV.y = v;

					res.intersection = to;
					res.distance = n;
					res.geometry = geo;
					res.renderable = rend;

					return true;
				}
			}
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