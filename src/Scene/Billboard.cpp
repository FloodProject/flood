/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"
#include "Scene/Billboard.h"
#include "Resources/Material.h"
#include "Scene/Entity.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

REFLECT_ENUM(BillboardType)
	ENUM(ScreenAligned)
	ENUM(WorldAligned)
	ENUM(AxisAligned)
REFLECT_ENUM_END()

REFLECT_CHILD_CLASS(Billboard, Component)
	FIELD_ENUM(BillboardType, billboardType)
REFLECT_CLASS_END()

//-----------------------------------//

Billboard::Billboard()
	: billboardType(BillboardType::WorldAligned)
{ }

//-----------------------------------//

Billboard::Billboard( BillboardType::Enum type )
	: billboardType(type)
{ }

//-----------------------------------//

void Billboard::update( float )
{
}

//-----------------------------------//

void Billboard::onPreRender( const Camera& camera )
{
#if 0
	TransformPtr transform = getEntity()->getTransform();
	const Matrix4x3& view = camera.getViewMatrix();

	Vector3 right( view.m11, view.m12, view.m13 );
	right.normalize();
	
	Vector3 up( view.m21, view.m22, view.m23 );
	up.normalize();

	Vector3 forward( view.m31, view.m32, view.m33 );
	forward.normalize();

	Matrix4x3 lookAt = transform->lookAt( -forward, up );
	
	Vector3 position = transform->getPosition();
	lookAt.tx = position.x;
	lookAt.ty = position.y;
	lookAt.tz = position.z;
	
	transform->setAbsoluteTransform( lookAt );
#endif

	const TransformPtr& transform = getEntity()->getTransform();
	const TransformPtr& camTransform = camera.getEntity()->getTransform();
	const Matrix4x3& camRot = camera.getViewMatrix();

	Vector3 look = transform->getPosition() + camRot * Vector3(0, 0, -1);
	Vector3 up = camRot * Vector3(0, 1, 0);
	
	Matrix4x3 lookAt = transform->lookAt(look, up);
	transform->setAbsoluteTransform( lookAt );
}

//-----------------------------------//

RenderablePtr Billboard::createDebugRenderable() const
{
	VertexBufferPtr vb = Allocate(VertexBuffer, AllocatorGetHeap());
	
	std::vector<Vector3> pos;
	std::vector<Vector3> colors;

	//TransformPtr cameraTransform = camera.getEntity()->getTransform();
	//Vector3 lookAt = transform->getPosition()-cameraTransform->getPosition();
	//
	//pos.push_back( transform->getPosition() );
	//pos.push_back( cameraTransform->getPosition() );

	colors.push_back( Vector3::Zero );
	colors.push_back( Vector3::Zero );
	
	//Vector3 up = camera->getLookAtVector().cross( Vector3::UnitY );

	//pos.push_back( up );
	//pos.push_back( up*3 );

	colors.push_back( Vector3::Zero );
	colors.push_back( Vector3::Zero );

	vb->set( VertexAttribute::Position, pos );
	vb->set( VertexAttribute::Color, colors );
	
	MaterialHandle material = MaterialCreate(AllocatorGetHeap(), "BillboardDebug");

	RenderablePtr rend = Allocate(Renderable, AllocatorGetHeap());
	rend->setPrimitiveType(PolygonType::Lines);
	rend->setVertexBuffer( vb );
	rend->setMaterial( material );

	return rend;
}

//-----------------------------------//

NAMESPACE_ENGINE_END