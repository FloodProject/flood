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

Transformable::Transformable()
	: v_scale( 1.0f, 1.0f, 1.0f )
{

}

//-----------------------------------//

Transformable::~Transformable()
{

}

//-----------------------------------//

void Transformable::translate( const math::Vector3& tr )
{
	translate( tr.x, tr.y, tr.z );
}

//-----------------------------------//

void Transformable::translate( float x, float y, float z )
{
	v_translate.x += x;
	v_translate.y += y;
	v_translate.z += z;

	//transform = transform * Matrix4x3::createTranslationMatrix( v_translate );
	//v_translate.zero();
}

//-----------------------------------//

const math::Vector3& Transformable::getPosition() const
{
	return v_translate;
}

//-----------------------------------//

void Transformable::setPosition( const math::Vector3 position )
{
	v_translate = position;
}

//-----------------------------------//

void Transformable::scale( float x, float y, float z )
{
	v_scale.x *= x;
	v_scale.y *= y;
	v_scale.z *= z;

	//transform = transform * Matrix4x3::createScaleMatrix( v_scale );
	//v_scale = math::Vector3( 1.0f, 1.0f, 1.0f );
}

//-----------------------------------//

void Transformable::scale( float uniform )
{
	scale( uniform, uniform, uniform );
}

//-----------------------------------//

void Transformable::scale( const math::Vector3& s )
{
	scale( s.x, s.y, s.z );
}

//-----------------------------------//

void Transformable::rotate( float xang, float yang, float zang )
{
	angles.xang += xang;
	angles.yang += yang;
	angles.zang += zang;

	//transform = transform * angles.getOrientationMatrix();
	//angles.identity();
}

//-----------------------------------//

void Transformable::rotate( const math::Vector3& rot )
{
	rotate( rot.x, rot.y, rot.z );
}

//-----------------------------------//

const math::EulerAngles& Transformable::getRotation() const
{
	return angles;
}

//-----------------------------------//

void Transformable::setRotation( math::EulerAngles& rot )
{
	this->angles = rot;
}

//-----------------------------------//

void Transformable::reset( )
{
	v_translate.zero();
	v_scale = math::Vector3( 1.0f, 1.0f, 1.0f );
	angles.identity();
}

//-----------------------------------//

void Transformable::setAbsoluteTransform( const math::Matrix4x3& matrix )
{
	absoluteLocalToWorld = matrix;
}

//-----------------------------------//

const math::Matrix4x3& Transformable::getAbsoluteTransform() const
{
	return absoluteLocalToWorld;
}

//-----------------------------------//

math::Matrix4x3 Transformable::getLocalTransform() const
{
	return Matrix4x3::createTranslationMatrix( v_translate )
		* angles.getOrientationMatrix()
		* Matrix4x3::createScaleMatrix( v_scale );
}

//-----------------------------------//

} } // end namespaces