/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"
#include "Event.h"

#ifdef VAPOR_PHYSICS_BULLET

#include "Physics/CapsuleShape.h"
#include "Physics/Convert.h"
#include "Physics/Body.h"

#include "Scene/Entity.h"
#include "Scene/Transform.h"

#include <BulletCollision/CollisionShapes/btCapsuleShape.h>

namespace vapor {

//-----------------------------------//

REFLECT_CHILD_CLASS(CapsuleShape, Shape)
	FIELD_PRIMITIVE(float, height)
	FIELD_PRIMITIVE(float, radius)
REFLECT_CLASS_END()

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