/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#include "vapor/scene/Transform.h"
#include "vapor/math/Math.h"

using namespace vapor::math;

namespace vapor { namespace scene {

//-----------------------------------//

const std::string& Transform::type = "Transform";

//-----------------------------------//

Transform::Transform()
	: v_scale( 1.0f ), isPhysicsDriven( false )
{

}

//-----------------------------------//

Transform::~Transform()
{

}

//-----------------------------------//

void Transform::translate( const math::Vector3& tr )
{
	translate( tr.x, tr.y, tr.z );
}

//-----------------------------------//

void Transform::translate( float x, float y, float z )
{
	v_translate.x += x;
	v_translate.y += y;
	v_translate.z += z;

	notify();

	//transform = transform * Matrix4x3::createTranslationMatrix( v_translate );
	//v_translate.zero();
}

//-----------------------------------//

void Transform::scale( float uniform )
{
	scale( uniform, uniform, uniform );
}

//-----------------------------------//

void Transform::scale( const math::Vector3& s )
{
	scale( s.x, s.y, s.z );
}

//-----------------------------------//

void Transform::scale( float x, float y, float z )
{
	v_scale.x *= x;
	v_scale.y *= y;
	v_scale.z *= z;

	notify();

	//transform = transform * Matrix4x3::createScaleMatrix( v_scale );
	//v_scale = math::Vector3( 1.0f, 1.0f, 1.0f );
}

//-----------------------------------//

void Transform::rotate( const math::Vector3& rot )
{
	rotate( rot.x, rot.y, rot.z );
}

//-----------------------------------//

void Transform::rotate( float xang, float yang, float zang )
{
	angles.xang += xang;
	angles.yang += yang;
	angles.zang += zang;

	notify();

	//transform = transform * angles.getOrientationMatrix();
	//angles.identity();
}

//-----------------------------------//

const math::EulerAngles& Transform::getRotation() const
{
	return angles;
}

//-----------------------------------//

void Transform::setRotation( math::EulerAngles& rot )
{
	this->angles = rot;

	notify();
}

//-----------------------------------//

const math::Vector3& Transform::getPosition() const
{
	return v_translate;
}

//-----------------------------------//

void Transform::setPosition( const math::Vector3 position )
{
	v_translate = position;
}

//-----------------------------------//

void Transform::lookAt( const math::Vector3& lookAtVector, const math::Vector3& upVector )
{
	Vector3 eye = Vector3( absoluteLocalToWorld.tx, absoluteLocalToWorld.ty, 
		absoluteLocalToWorld.tz );

	Vector3 zaxis = ( lookAtVector - eye ).normalize();
	Vector3	xaxis = upVector.cross(zaxis).normalize();
	Vector3	yaxis = zaxis.cross(xaxis);

	Matrix4x3 m;
	m.m11 = xaxis.x;
	m.m12 = xaxis.y;
	m.m13 = xaxis.z;

	m.m21 = yaxis.x;
	m.m22 = yaxis.y;
	m.m23 = yaxis.z;

	m.m31 = zaxis.x;
	m.m32 = zaxis.y;
	m.m33 = zaxis.z;

	m.tx = -xaxis.dot(eye);
	m.ty = -yaxis.dot(eye);
	m.tz = -zaxis.dot(eye);

	//viewMatrix = m * getNode()->getTransform()->getLocalTransform();

	//glMatrixMode( GL_MODELVIEW );
	//glLoadIdentity();

	//gluLookAt( eye.x, eye.y, eye.z,
	//	lookAtVector.x, lookAtVector.y, lookAtVector.z,
	//	upVector.x, upVector.y, upVector.z );

	//float test[16];
	//glGetFloatv( GL_MODELVIEW_MATRIX, test );

	//Matrix4x3 glView;

	//glView.m11 = test[0];
	//glView.m12 = test[1];
	//glView.m13 = test[2];
	//
	//glView.m21 = test[4];
	//glView.m22 = test[5];
	//glView.m23 = test[6];

	//glView.m31 = test[8];
	//glView.m32 = test[9];
	//glView.m33 = test[10];

	//glView.tx = test[12];
	//glView.ty = test[13];
	//glView.tz = test[14];
}

//-----------------------------------//

void Transform::reset( )
{
	v_translate.zero();
	v_scale = math::Vector3( 1.0f, 1.0f, 1.0f );
	angles.identity();
}

//-----------------------------------//

void Transform::setAbsoluteTransform( const math::Matrix4x3& matrix )
{
	absoluteLocalToWorld = matrix;
}

//-----------------------------------//

const math::Matrix4x3& Transform::getAbsoluteTransform() const
{
	return absoluteLocalToWorld;
}

//-----------------------------------//

math::Matrix4x3 Transform::getLocalTransform() const
{
	return Matrix4x3::createTranslationMatrix( v_translate )
		* angles.getOrientationMatrix()
		* Matrix4x3::createScaleMatrix( v_scale );
}

//-----------------------------------//

const std::string& Transform::getType() const
{
	return Transform::type;
}

//-----------------------------------//

void Transform::update( float /*delta*/ )
{

}

//-----------------------------------//

void Transform::notify()
{
	if( onTransform.empty() ) 
	{
		return;
	}

	onTransform();
}

//-----------------------------------//

} } // end namespaces