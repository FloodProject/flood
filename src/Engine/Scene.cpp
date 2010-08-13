/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/scene/Scene.h"
#include "vapor/scene/Transform.h"
#include "vapor/scene/Tags.h"

namespace vapor {

//-----------------------------------//

BEGIN_CLASS_PARENT(Scene, Group)
END_CLASS()

//-----------------------------------//

Scene::Scene()
	: Group("Scene")
{ }

//-----------------------------------//

bool Scene::doRayBoxQuery( const Ray& ray, RayBoxQueryResult& res ) const
{
	RayBoxQueryList list;
	if( !doRayBoxQuery( ray, list ) )
		return false;

	res = list.front();	
	return true;
}

//-----------------------------------//

bool sortResult(const RayBoxQueryResult& lhs, const RayBoxQueryResult& rhs)
{
	return lhs.distance < rhs.distance;
}

bool Scene::doRayBoxQuery( const Ray& ray, RayBoxQueryList& list, bool all ) const
{
	// Do some ray tracing to find a collision.
	foreach( const NodePtr& node, getChildren() )
	{
		// No need to pick invisible nodes.
		if( !node->isVisible() || node->getTag(Tags::NonPickable) )
			continue;

		const TransformPtr& transform = node->getTransform();
		
		if( !transform )
			continue;

		const AABB& aabb = transform->getWorldBoundingVolume();
			
		float distance;
		if( aabb.intersects(ray, distance) )
		{
			RayBoxQueryResult res;
			res.node = node;
			res.distance = distance;

			list.push_back( res );

			if( !all ) break;
		}
	}

	// Sort the results by distance.
	std::sort( list.begin(), list.end(), &sortResult );

	return !list.empty();
}

//-----------------------------------//

bool Scene::doRayTriangleQuery( const Ray& ray, RayTriangleQueryResult& res ) const
{
	// Perform ray casting to find the nodes.
	RayBoxQueryList list;
	doRayBoxQuery( ray, list );

	foreach( const RayBoxQueryResult& query, list )
	{
		if( doRayTriangleQuery( ray, res, query.node ) )
			return true;
	}

	return false;
}

//-----------------------------------//

bool Scene::doRayTriangleQuery( const Ray& ray, RayTriangleQueryResult& res,
							    const NodePtr& node ) const
{
	// Down to triangle picking.	
	foreach( const GeometryPtr& geo, node->getGeometry() )
	{
		if( !geo )
			continue;

		// Let's do a rough bounding volume level intersection test on each
		// individual geometry of the node. This helps cut the number of 
		// collisions tests in nodes with lots of geometry components.

		const AABB& bv = geo->getBoundingVolume();

		float distance;
		
		if( !bv.intersects(ray, distance) )
			continue;

		foreach( const RenderablePtr& rend, geo->getRenderables() )
		{
			// This picking method only works on triangles.

			if( !rend )
				continue;

			if( rend->getPrimitiveType() != PolygonPrimitive::Triangles )
				continue;

			const VertexBufferPtr& vb = rend->getVertexBuffer();
			
			if( !vb )
				continue;

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

				Vector3 to; float n;
				if( ray.intersects( tri, to, n ) )
				{					
					for( byte i = 0; i < 3; i++ )
						res.triangle[i] = tri[i];					
				
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

NodePtr Scene::getEntity( const std::string& name ) const
{
	foreach( const NodePtr& node, children )
	{
		if( node->getName() == name )
			return node;
	}

	return NodePtr();
}

//-----------------------------------//

} // end namespace