/************************************************************************
*
* vaporEngine (2008-2010)
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

	initiated = false;
	inWorld = true;
	firstUpdate = true;
}

//-----------------------------------//

Body::Body(float mass, hkpMotion::MotionType motion)
{
	physicsManager = physics::PhysicsManager::getInstancePtr();
	
	info.m_mass = mass;
	info.m_motionType = motion;
	
	initiated = false;
	inWorld = true;
	firstUpdate = true;
}

//-----------------------------------//

void Body::init()
{

	initiated = true;
	transform = getNode()->getTransformPtr();
	transform->isPhysicsDriven = true;

	const math::AABB& bb = (*((getNode()->getGeometry())[0])).getBoundingVolume();
	
	// Get a box shape from AABB
	shape = getShape( bb ); 
	
	// Send the transform of the node to Havok
	setTransform( info );

	hkpMassProperties massProperties;

	info.m_centerOfMass = convertVector(transform->getPosition());
	
	hkVector4 boxSize = shape->getHalfExtents();
	boxSize.mul4(2);
	hkReal boxMass = info.m_mass;
	
	hkpInertiaTensorComputer::computeBoxVolumeMassProperties(boxSize, boxMass, massProperties);
	
	info.m_inertiaTensor = massProperties.m_inertiaTensor;
	info.m_shape = shape;
	
	body = new hkpRigidBody(info);
	physicsManager->addEntity(body);
}

//-----------------------------------//

Body::~Body()
{
	if(inWorld)
		physicsManager->removeEntity(body);

	body->removeReference();
	shape->removeReference();
}

//-----------------------------------//

const std::string& Body::getType() const
{
	return Body::type;
}

//-----------------------------------//

void Body::update( float delta )
{
	if( firstUpdate )
	{
		init();
		firstUpdate = false;
	}

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

hkpBoxShape * Body::getShape(const math::AABB& bb)
{
	float hx, hy, hz;
	
	hx = ::abs(((bb.getMaximum()).x - (bb.getMinimum()).x) / 2.0f);
	hy = ::abs(((bb.getMaximum()).y - (bb.getMinimum()).y) / 2.0f);
	hz = ::abs(((bb.getMaximum()).z - (bb.getMinimum()).z) / 2.0f);
	
	hkVector4 v(hx, hy, hz);
	hkpBoxShape * box = new hkpBoxShape(v);
	
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
	rot.renormalize();
	
	hkQuaternion rotation(rot);
	info.m_rotation = rotation;	
}

//-----------------------------------//

void Body::addToWorld()
{
	if(!inWorld && initiated) physicsManager->addEntity(body); 
}

//-----------------------------------//

void Body::removeFromWorld()
{
	if(inWorld && initiated) physicsManager->removeEntity(body);
}

//-----------------------------------//

void Body::setLinearVelocity(const math::Vector3 &lVel)
{
	if(initiated)
	{
		hkVector4 v = convertVector(lVel);
		body->setLinearVelocity(v);
	}
}

//-----------------------------------//

void Body::setAngularVelocity(const math::Vector3 &aVel)
{
	if(initiated)
	{
		hkVector4 v = convertVector(aVel);
		body->setAngularVelocity(v);
	}
}

//-----------------------------------//

void Body::applyForce(const math::Vector3 &force)
{
	if(initiated)
	{
		hkVector4 f = convertVector(force);
		body->applyForce(physicsManager->del, f);
	}
}

//-----------------------------------//

void Body::applyTorque(const math::Vector3 &torque)
{
	if(initiated)
	{
		hkVector4 t = convertVector(torque);
		body->applyTorque(physicsManager->del, t);
	}
}

//-----------------------------------//

void Body::applyLinearImpulse(const math::Vector3 &imp)
{
	if(initiated)
	{
		hkVector4 i = convertVector(imp);
		body->applyLinearImpulse(i);
	}
}

//-----------------------------------//

hkVector4 Body::convertVector(const math::Vector3 &v)
{
	hkVector4 u(v.x, v.y, v.z);
	return u;
}

//-----------------------------------//

void  Body::setPosition (const math::Vector3 &position)
{
	if(initiated)
	{
		hkVector4 p = convertVector(position);
		body->setPosition(p);
	}
}


//-----------------------------------//

void  Body::setPositionAndRotation (const math::Matrix4x3 &m)
{
	if(initiated)
	{
		hkVector4 c1(m.m11, m.m21, m.m31);
		hkVector4 c2(m.m12, m.m22, m.m32);
		hkVector4 c3(m.m13, m.m23, m.m33);
		hkVector4 p(m.tx, m.ty, m.tz);
		hkRotation rot;
		rot.setCols(c1, c2, c3);
		rot.renormalize();
		hkQuaternion r(rot);
		body->setPositionAndRotation(p, r);
	}
}

//-----------------------------------//
} } // end namespaces
