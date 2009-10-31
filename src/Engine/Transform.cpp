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

namespace vapor {
	namespace scene {

//-----------------------------------//

const std::string& Transform::type = "Transform";

//-----------------------------------//

Transform::Transform()
	: v_scale( 1.0f, 1.0f, 1.0f )
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

	//transform = transform * Matrix4x3::createTranslationMatrix( v_translate );
	//v_translate.zero();
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

void Transform::scale( float x, float y, float z )
{
	v_scale.x *= x;
	v_scale.y *= y;
	v_scale.z *= z;

	//transform = transform * Matrix4x3::createScaleMatrix( v_scale );
	//v_scale = math::Vector3( 1.0f, 1.0f, 1.0f );
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

void Transform::rotate( float xang, float yang, float zang )
{
	angles.xang += xang;
	angles.yang += yang;
	angles.zang += zang;

	//transform = transform * angles.getOrientationMatrix();
	//angles.identity();
}

//-----------------------------------//

void Transform::rotate( const math::Vector3& rot )
{
	rotate( rot.x, rot.y, rot.z );
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

} } // end namespaces