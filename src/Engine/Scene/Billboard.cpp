/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Engine/API.h"
#include "Engine/Scene/Billboard.h"
#include "Graphics/Resources/Material.h"
#include "Engine/Scene/Entity.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

REFLECT_ENUM(BillboardType)
	ENUM(ScreenAligned)
	ENUM(WorldAligned)
	ENUM(AxisAligned)
REFLECT_ENUM_END()

REFLECT_CHILD_CLASS(Billboard, Component)
	FIELD_ENUM(4, BillboardType, billboardType)
REFLECT_CLASS_END()

//-----------------------------------//

Billboard::Billboard() : billboardType(BillboardType::ScreenAligned)
{
}

//-----------------------------------//

Billboard::Billboard( BillboardType type ) : billboardType(type)
{
}

//-----------------------------------//

void Billboard::update( float )
{
}

//-----------------------------------//

void Billboard::onPreRender( const Camera& camera )
{
	Transform* transform = getEntity()->getTransform().get();
	
	const Transform* camTransform = camera.getEntity()->getTransform().get();
	const Matrix4x3& camView = camera.getViewMatrix();

#if 0
	Vector3 look = camTransform->getPosition() - transform->getPosition();
	look.normalize();

	Vector3 cameraUp( camView.m21, camView.m22, camView.m23 ); 

	Matrix4x3 lookAt = transform->lookAt(look, cameraUp);
#endif

	Matrix4x3 lookAt = camView.transpose();
	lookAt.tx = transform->getPosition().x;
	lookAt.ty = transform->getPosition().y;
	lookAt.tz = transform->getPosition().z;

	transform->setAbsoluteTransform( lookAt );
}

//-----------------------------------//

NAMESPACE_ENGINE_END