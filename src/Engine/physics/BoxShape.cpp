/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include "vapor/physics/BoxShape.h"
#include "vapor/math/BoundingBox.h"
#include "vapor/scene/Node.h"
#include "vapor/scene/Transform.h"

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

btVector3 toBulletBox(const AABB& box)
{
	return btVector3(
		box.max.x,
		box.max.y,
		box.max.z );
}

//-----------------------------------//

void BoxShape::update( double delta )
{
	const TransformPtr& transform = getNode()->getTransform();
	
	if( !transform )
		return;
	
	const AABB& box = transform->getBoundingVolume();
	
	if( !boxShape )
	{
		boxShape = new btBoxShape(toBulletBox(box));
	}
}

//-----------------------------------//

btCollisionShape* const BoxShape::getBulletShape() const
{
	return boxShape;
}

//-----------------------------------//

} // end namespace