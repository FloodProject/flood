/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#ifdef VAPOR_PHYSICS_BULLET

#include "physics/Shape.h"

FWD_DECL_SHARED(Geometry)

class btBvhTriangleMeshShape;
class btTriangleIndexVertexArray;
class btTriangleMesh;
class btGImpactMeshShape;

namespace vapor {

//-----------------------------------//

class VAPOR_API MeshShape : public Shape
{
	DECLARE_CLASS_()

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

TYPEDEF_SHARED_POINTER_FROM_TYPE( MeshShape );
TYPEDEF_SHARED_WEAK_POINTER_FROM_TYPE( MeshShape );

//-----------------------------------//

} // end namespace

#endif