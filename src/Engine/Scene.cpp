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

using namespace vapor::math;
using namespace vapor::render;

namespace vapor { namespace scene {

//-----------------------------------//

Scene::Scene() : Group( "Scene" )
{

}

//-----------------------------------//

Scene::~Scene()
{

}

//-----------------------------------//

bool Scene::doRayBoxQuery( const Ray& ray, RayBoxQueryList& list ) const
{
	// Do some ray tracing to find a collision.
	foreach( const NodePtr& node, getChildren() )
	{
		const AABB& aabb = node->getTransform()->getWorldBoundingVolume();
			
		float distance;
		if( aabb.intersects( ray, distance ) )
		{
			RayBoxQueryResult res;
			res.node = node;
			res.distance = distance;

			list.push_back( res );
		}
	}

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

bool Scene::doRayTriangleQuery( const Ray& ray, RayTriangleQueryResult& res, const NodePtr& node ) const
{
	// Down to triangle picking.	
	foreach( const GeometryPtr& geo, node->getGeometry() )
	{
		if( !geo ) continue;

		foreach( const RenderablePtr& rend, geo->getRenderables() )
		{
			// This picking method only works on triangles.
			if( !rend ) continue;
			if( rend->getPrimitiveType() != Primitive::Triangles) continue;

			const VertexBufferPtr& vb = rend->getVertexBuffer();
			if( !vb ) continue;

			const std::vector<Vector3>& vertices = vb->getVertices();
			if( vertices.size() < 3 ) continue;

			assert( !rend->getIndexBuffer() );

			for( uint i = 0; i < vertices.size(); i += 3 )
			{
				Vector3 tri[3];
				tri[0] = vertices[i];
				tri[1] = vertices[i+1];
				tri[2] = vertices[i+2];

				Vector3 to; float n;
				if( ray.intersects( tri, to, n ) )
				{
					res.geometry = geo;
					
					for( byte i = 0; i < 3; i++ )
						res.triangle[i] = tri[i];
					
					res.distance = n;

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

	//MatrixStack transformStack;
	//transformStack.push( Matrix4x3::Identity );
	//updateTransformAndBV( shared_from_this(), transformStack );
}

//-----------------------------------//

//void Scene::updateTransformAndBV( NodePtr /*node*/, MatrixStack& /*transformStack*/ )
//{
	// TODO: benckmark and profile this, smells slow
	//PROFILE;

	//// on the way down part
	//TransformPtr transform = node->getTransform();

	//// this is used to know if we need to pop a matrix from the
	//// stack on the way up part, because some nodes will not have
	//// pushed matrices on the stack on the way down part.
	//bool needsPop = false;
 
	//// concatenate this matrix on the stack with the top most
	//// there will be at least one matrix in stack, top most is identity
	//if ( transform )
	//{
	//	transformStack.push( transform->getLocalTransform() * transformStack.top() );

	//	//if( !transform->isPhysicsDriven )
	//		transform->setAbsoluteTransform( transformStack.top() );
	//	
	//	needsPop = true;
	//}

	//GroupPtr group( std::dynamic_pointer_cast< Group >( node ) );

	//if( group )
	//{
	//	foreach( NodePtr gnode, group->getChildren() )
	//	{
	//		updateTransformAndBV( gnode, transformStack );
	//	}
	//}

	//// on the way up part
	//node->update( delta );
	//    
	//// remove the matrix from the stack
	//if ( needsPop )
	//{
	//	transformStack.pop();
	//}
//}

//-----------------------------------//

Node* Scene::getEntityPtr( const std::string& name ) const
{
	return getEntity(name).get();
}

//-----------------------------------//

NodePtr Scene::getEntity( const std::string& name ) const
{
	//if( getName() == name ) return shared_from_this();

	foreach( const NodePtr& node, getChildren() )
	{
		if( node->getName() == name )
			return node;
	}

	return NodePtr();
}

//-----------------------------------//

} } // end namespaces