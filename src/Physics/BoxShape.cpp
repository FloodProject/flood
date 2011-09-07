/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"

#ifdef ENABLE_PHYSICS_BULLET

#include "Physics/BoxShape.h"
#include "Physics/Convert.h"

#include "Scene/Entity.h"
#include "Scene/Transform.h"

#include <BulletCollision/CollisionShapes/btBoxShape.h>

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(BoxShape, Shape)
REFLECT_CLASS_END()

//-----------------------------------//

BoxShape::BoxShape() : boxShape(nullptr)
{
}

//-----------------------------------//

BoxShape::~BoxShape()
{
	removeBody();
	Deallocate(boxShape);
}

//-----------------------------------//

void BoxShape::update( float delta )
{
	if( boxShape ) return;

	const TransformPtr& transform = getEntity()->getTransform();
	if( !transform ) return;
	
	const BoundingBox& box = transform->getBoundingVolume();
	const Vector3& scale = transform->getScale();
	
	boxShape = AllocateThis(btBoxShape, Convert::toBullet(box));
	boxShape->setLocalScaling(Convert::toBullet(scale));
}

//-----------------------------------//

btCollisionShape* const BoxShape::getBulletShape() const
{
	return boxShape;
}

//-----------------------------------//

NAMESPACE_ENGINE_END

#endif