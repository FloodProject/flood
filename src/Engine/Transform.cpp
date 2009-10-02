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
	v_translate.x += tr.x;
	v_translate.y += tr.y;
	v_translate.z += tr.z;

	transform = transform * getTranslationMatrix( v_translate );
	v_translate.zero();
}

//-----------------------------------//

void Transformable::scale( float x, float y, float z )
{
	v_scale.x *= x;
	v_scale.y *= y;
	v_scale.z *= z;
}

//-----------------------------------//

void Transformable::scale( float uniform )
{
	scale( uniform, uniform, uniform );
}

//-----------------------------------//

void Transformable::rotate( float xang, float yang, float zang )
{
	angles.xang += xang;
	angles.yang += yang;
	angles.zang += zang;

	transform = transform * angles.getOrientationMatrix();
	angles.identity();
}

//-----------------------------------//

void Transformable::reset( )
{
	angles.identity();
	v_translate.zero();
	v_scale.zero();
}

//-----------------------------------//

void Transformable::setAbsoluteTransform( const math::Matrix4& matrix )
{
	absoluteLocalToWorld = matrix;
}

//-----------------------------------//

const math::Matrix4& Transformable::getAbsoluteTransform() const
{
	return absoluteLocalToWorld;
}

//-----------------------------------//

math::Matrix4 Transformable::getLocalTransform() const
{
	//transform = transform * getTranslationMatrix( v_translate );
	//transform = transform * angles.getOrientationMatrix();
	//transform = transform * getScaleMatrix( v_scale );

	return transform/* * angles.getOrientationMatrix()*/;
}

//-----------------------------------//

math::Matrix4 Transformable::getScaleMatrix( math::Vector3 v ) const
{
	Matrix4 s;

	s.m11 = v.x;
	s.m22 = v.y;
	s.m33 = v.z;

	return s;
}

//-----------------------------------//

math::Matrix4 Transformable::getTranslationMatrix( math::Vector3 v ) const
{
	Matrix4 s;

	s.tx = v.x;
	s.ty = v.y;
	s.tz = v.z;

	return s;
}

//-----------------------------------//

} } // end namespaces