/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/physics/BoxShape.h"
#include "vapor/physics/Convert.h"

#include "vapor/scene/Node.h"
#include "vapor/scene/Transform.h"

#include <BulletCollision/CollisionShapes/btBoxShape.h>

namespace vapor {

//-----------------------------------//

BEGIN_CLASS_PARENT(BoxShape, Shape)
END_CLASS()

//-----------------------------------//

BoxShape::BoxShape()
	: boxShape(nullptr)
{ }

//-----------------------------------//

BoxShape::~BoxShape()
{
	delete boxShape;
}

//-----------------------------------//

void BoxShape::update( double delta )
{
	if( boxShape )
		return;

	const TransformPtr& transform = getNode()->getTransform();
	
	if( !transform )
		return;
	
	const BoundingBox& box = transform->getBoundingVolume();
	const Vector3& scale = transform->getScale();
	
	boxShape = new btBoxShape(Convert::toBullet(box));
	boxShape->setLocalScaling(Convert::toBullet(scale));
}

//-----------------------------------//

btCollisionShape* const BoxShape::getBulletShape() const
{
	return boxShape;
}

//-----------------------------------//

} // end namespace