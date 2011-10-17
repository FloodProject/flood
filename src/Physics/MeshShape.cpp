/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"

#ifdef ENABLE_PHYSICS_BULLET

#include "Physics/MeshShape.h"
#include "Physics/Convert.h"
#include "Scene/Entity.h"
#include "Scene/Transform.h"
#include "Scene/Geometry.h"

#include <BulletCollision/CollisionShapes/btTriangleMesh.h>
#include <BulletCollision/CollisionShapes/btTriangleIndexVertexArray.h>
#include <BulletCollision/CollisionShapes/btBvhTriangleMeshShape.h>
#include <BulletCollision/Gimpact/btGImpactShape.h>

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(MeshShape, Shape)
REFLECT_CLASS_END()

//-----------------------------------//

MeshShape::MeshShape()
	: meshShape(nullptr)
{ }

//-----------------------------------//

MeshShape::~MeshShape()
{
	removeBody();
	Deallocate(meshShape);
}

//-----------------------------------//

#if 0
btTriangleIndexVertexArray* MeshShape::convertMesh()
{
	btTriangleIndexVertexArray* vertexArray;
	vertexArray = new btTriangleIndexVertexArray();

	const EntityPtr& node = getEntity();
	
	foreach( const GeometryPtr& geometry, node->getGeometry() )
	{
		const RenderableVector& rends = geometry->getRenderables();

		foreach( const RenderablePtr& rend, rends )
		{
			if( rend->getPrimitiveType() != PolygonType::Triangles )
				continue;

			GeometryBuffer gb = rend->getVertexBuffer();
			const std::vector<Vector3>& pos = vb->getAttribute( VertexAttribute::Position );
			
			IndexBufferPtr ib = rend->getIndexBuffer();
			const std::vector<ushort>& indices = ib->getIndices16();

 		//	btIndexedMesh* indexMesh = ;

			//foreach( const Vector3& vec, pos )
			//{
			//	//indexMesh.m_numTriangles
			//}

			//vertexArray->addIndexedMesh(indexMesh);
		}
	}

	return vertexArray;
}
#endif

btTriangleMesh* MeshShape::convertMesh()
{
#if 0
	btTriangleMesh* mesh = AllocateThis(btTriangleMesh, false);

	const std::vector<GeometryPtr>& geometries = entity->getGeometry();

	for( size_t i = 0; i < geometries.size(); i++ )
	{
		const GeometryPtr& geometry = geometries[i];
		const RenderableVector& rends = geometry->getRenderables();

		for( uint e = 0; e < rends.size(); e++ )
		{
			const RenderablePtr& rend = rends[e];

			if( rend->getPrimitiveType() != PolygonType::Triangles )
				continue;

			GeometryBuffer gb = rend->getVertexBuffer();
			const std::vector<Vector3>& pos = vb->getAttribute( VertexAttribute::Position );
			
			IndexBufferPtr ib = rend->getIndexBuffer();

			if( !ib )
			{		
				for( size_t i = 0; i < pos.size(); i += 3 )
				{
					btVector3 vertex0 = Convert::toBullet(pos[i]);
					btVector3 vertex1 = Convert::toBullet(pos[i+1]);
					btVector3 vertex2 = Convert::toBullet(pos[i+2]);
					
					mesh->addTriangle(vertex0, vertex1, vertex2);
				}
			}
			else
			{
				const std::vector<ushort>& ind = ib->getIndices16();

				for( size_t i = 0; i < ind.size(); i += 3 )
				{
					btVector3 vertex0 = Convert::toBullet(pos[ind[i]]);
					btVector3 vertex1 = Convert::toBullet(pos[ind[i+1]]);
					btVector3 vertex2 = Convert::toBullet(pos[ind[i+2]]);
					
					mesh->addTriangle(vertex0, vertex1, vertex2);
				}
			}
		}
	}
	return mesh;
#endif
	return nullptr;
}

//-----------------------------------//

void MeshShape::update( float delta )
{
	return;

	if( meshShape ) return;	

	meshShape = AllocateThis(btBvhTriangleMeshShape, convertMesh(), true);
	
	const TransformPtr& transform = getEntity()->getTransform();
	const Vector3& scale = transform->getScale();
	meshShape->setLocalScaling(Convert::toBullet(scale));
}

//-----------------------------------//

btCollisionShape* const MeshShape::getBulletShape() const
{
	return meshShape;
}

//-----------------------------------//

NAMESPACE_ENGINE_END

#endif