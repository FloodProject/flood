/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#include "vapor/scene/Body.h"
#include "vapor/math/Math.h"

using namespace vapor::math;

namespace vapor { namespace scene {

//-----------------------------------//

const std::string& Body::type = "Body";

//-----------------------------------//

Body::Body()
{
	physicsManager = physics::PhysicsManager::getInstancePtr();

	// Construct a Box shape from the bounding box
	transform = getNode()->getTransformPtr();
	const math::AABB& bb = (*((getNode()->getGeometry())[0])).getBoundingVolume(); 
	
	hkpBoxShape shape = getShape(bb); 
	
	// 
	hkpRigidBodyCinfo info;
	setTransform(info);
	hkpInertiaTensorComputer::setShapeVolumeMassProperties(&shape, info.m_mass, info);
	body = new hkpRigidBody(info);
	
	physicsManager->addEntity(body);
	inWorld = true;
}

//-----------------------------------//

Body::Body(float mass, hkpMotion::MotionType motion)
{

}

//-----------------------------------//

Body::~Body()
{
	if(inWorld) 
		physicsManager->removeEntity(body);

	body->removeReference();
}

//-----------------------------------//

const std::string& Body::getType() const
{
	return Body::type;
}

//-----------------------------------//

void Body::update( float delta )
{
	hkRotation rot;
	rot.set(body->getRotation());
	
	math::Matrix4x3 m;
	
	m.m11 = (rot.getColumn(0))(0);
	m.m21 = (rot.getColumn(0))(1);
	m.m31 = (rot.getColumn(0))(2);

	m.m12 = (rot.getColumn(1))(0);
	m.m22 = (rot.getColumn(1))(1);
	m.m32 = (rot.getColumn(1))(2);

	m.m13 = (rot.getColumn(2))(0);
	m.m23 = (rot.getColumn(2))(1);
	m.m33 = (rot.getColumn(2))(2);

	m.tx = (body->getPosition())(0);
	m.ty = (body->getPosition())(1);
	m.tz = (body->getPosition())(2);
	
	transform->setAbsoluteTransform(m);
}

//-----------------------------------//

hkpBoxShape Body::getShape(const math::AABB& bb)
{
	float hx, hy, hz;
	
	hx = (bb.getMaximum().x - bb.getMinimum().x) / 2.0f;
	hy = (bb.getMaximum().y - bb.getMinimum().y) / 2.0f;
	hz = (bb.getMaximum().z - bb.getMinimum().z) / 2.0f;
	
	hkVector4 v(hx, hy, hz);
	hkpBoxShape box(v);
	
	return box;
}

//-----------------------------------//

void Body::setTransform(hkpRigidBodyCinfo& info)
{
	Vector3 p = transform->getPosition();
	hkVector4 pos(p.x, p.y, p.z);
	info.m_position = pos;
	
	Matrix4x3 r = transform->getAbsoluteTransform();
	hkVector4 c1(r.m11, r.m21, r.m31);
	hkVector4 c2(r.m12, r.m22, r.m32);
	hkVector4 c3(r.m13, r.m23, r.m33);
	hkRotation rot;
	rot.setCols(c1, c2, c3);
	hkQuaternion rotation(rot);
	info.m_rotation = rotation;	
}

//-----------------------------------//

} } // end namespaces