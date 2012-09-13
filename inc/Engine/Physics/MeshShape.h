/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#ifdef ENABLE_PHYSICS_BULLET

#include "Engine/Physics/Shape.h"

FWD_DECL_INTRUSIVE(Geometry)

class btBvhTriangleMeshShape;
class btTriangleIndexVertexArray;
class btTriangleMesh;
class btGImpactMeshShape;

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

API_ENGINE REFLECT_DECLARE_CLASS(MeshShape)

class API_ENGINE MeshShape : public Shape
{
	REFLECT_DECLARE_OBJECT(MeshShape)

public:

	MeshShape();
	virtual ~MeshShape();

	// Updates the shape.
	void update( float delta );

	// Gets the internal Bullet shape.
	btCollisionShape* const getBulletShape() const;

protected:

	// Converts the geometry to Bullet format.
	btTriangleMesh* convertMesh();

	// Bullet box shape.
	btBvhTriangleMeshShape* meshShape;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( MeshShape );

//-----------------------------------//

NAMESPACE_ENGINE_END

#endif