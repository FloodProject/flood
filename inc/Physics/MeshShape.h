/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#ifdef VAPOR_PHYSICS_BULLET

#include "Physics/Shape.h"

FWD_DECL_INTRUSIVE(Geometry)

class btBvhTriangleMeshShape;
class btTriangleIndexVertexArray;
class btTriangleMesh;
class btGImpactMeshShape;

namespace vapor {

//-----------------------------------//

REFLECT_DECLARE_CLASS(MeshShape)

class VAPOR_API MeshShape : public Shape
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

} // end namespace

#endif