/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "Event.h"

#ifdef VAPOR_PHYSICS_BULLET

#include "vapor/physics/CapsuleShape.h"
#include "vapor/physics/Convert.h"
#include "vapor/physics/Body.h"

#include "vapor/scene/Entity.h"
#include "vapor/scene/Transform.h"

#include <BulletCollision/CollisionShapes/btCapsuleShape.h>

namespace vapor {

//-----------------------------------//

BEGIN_CLASS_PARENT(CapsuleShape, Shape)
	FIELD_PRIMITIVE(float, height)
	FIELD_PRIMITIVE(float, radius)
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
	removeBody();
	delete capsuleShape;
}

//-----------------------------------//

void CapsuleShape::update( float delta )
{
	if( capsuleShape )
		return;

	const TransformPtr& transform = getEntity()->getTransform();
	
	if( !transform )
		return;
	
	const Vector3& scale = transform->getScale();

	capsuleShape = new btCapsuleShape(radius, height);
	capsuleShape->setLocalScaling(Convert::toBullet(scale));
}

//-----------------------------------//

btCollisionShape* const CapsuleShape::getBulletShape() const
{
	return capsuleShape;
}

//-----------------------------------//

void CapsuleShape::onFieldChanged(const Field& field)
{
	if( !capsuleShape )
		return;

	const BodyPtr& body = getEntity()->getComponent<Body>();

	if( body )
		body->removeWorld();

	btVector3 dimensions(radius, 0.5*height, radius);
	capsuleShape->setImplicitShapeDimensions(dimensions);

	if( body )
		body->addWorld();
}

//-----------------------------------//

} // end namespace

#endif