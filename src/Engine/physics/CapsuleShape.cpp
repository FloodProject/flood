/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/physics/CapsuleShape.h"
#include "vapor/physics/Convert.h"

#include "vapor/scene/Node.h"
#include "vapor/scene/Transform.h"

#include <BulletCollision/CollisionShapes/btCapsuleShape.h>

namespace vapor {

//-----------------------------------//

BEGIN_CLASS_PARENT(CapsuleShape, Shape)
	FIELD_PRIMITIVE(CapsuleShape, float, height)
	FIELD_PRIMITIVE(CapsuleShape, float, radius)
END_CLASS()

//-----------------------------------//

CapsuleShape::CapsuleShape()
	: capsuleShape(nullptr)
	, radius(1.0f)
	, height(3.0f)
{ }

//-----------------------------------//

CapsuleShape::~CapsuleShape()
{
	delete capsuleShape;
}

//-----------------------------------//

void CapsuleShape::update( double delta )
{
	if( capsuleShape )
		return;

	const TransformPtr& transform = getNode()->getTransform();
	
	if( !transform )
		return;
	
	const BoundingBox& box = transform->getBoundingVolume();
	const Vector3& scale = transform->getScale();

	radius = box.max.x - box.min.x;
	height = box.max.y - box.min.y;
	
	capsuleShape = new btCapsuleShape(radius * 0.8f, height * 0.8f);
	capsuleShape->setLocalScaling(Convert::toBullet(scale));
}

//-----------------------------------//

btCollisionShape* const CapsuleShape::getBulletShape() const
{
	return capsuleShape;
}

//-----------------------------------//

} // end namespace