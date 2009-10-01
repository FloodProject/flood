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
	//: translation(nullptr), orientation(nullptr)
{
	
}

//-----------------------------------//

Transformable::~Transformable()
{
	//delete translation;
}

//-----------------------------------//

void Transformable::translate( const math::Vector3& tr )
{
	transform.tx += tr.x;
	transform.ty += tr.y;
	transform.tz += tr.z;
}

//-----------------------------------//

void Transformable::scale( float x, float y, float z )
{
	transform.m11 *= x;
	transform.m22 *= y;
	transform.m33 *= z;
}

//-----------------------------------//

void Transformable::scale( float uniform )
{
	scale( uniform, uniform, uniform );
}

//-----------------------------------//

void Transformable::rotate( float xang, float yang, float zang )
{
	debug( "%f %f", xang, yang );

	rotateX( xang );
	rotateY( yang );
}

//-----------------------------------//

void Transformable::rotateX( float ang )
{
	Matrix4 newRotation;

	newRotation.m11 = 1;

	newRotation.m22 = math::cosf( ang );
	newRotation.m23 = math::sinf( ang );

	newRotation.m32 = -math::sinf( ang );
	newRotation.m33 = math::cosf( ang );

	transform = transform * newRotation;
}

//-----------------------------------//

void Transformable::rotateY( float ang )
{
	Matrix4 newRotation;

	newRotation.m11 = math::cosf( ang );
	newRotation.m13 = -math::sinf( ang );

	newRotation.m22 = 1;

	newRotation.m31 = math::sinf( ang );
	newRotation.m33 = math::cosf( ang );

	transform = transform * newRotation;
}

//-----------------------------------//

void Transformable::rotateZ( float ang )
{
	Matrix4 newRotation;

	newRotation.m11 = math::cosf( ang );
	newRotation.m12 = math::sinf( ang );

	newRotation.m21 = -math::sinf( ang );
	newRotation.m22 = math::cosf( ang );

	newRotation.m33 = 1;

	transform = transform * newRotation;
}

//-----------------------------------//

void Transformable::reset( )
{
	transform.identity();
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

const math::Matrix4& Transformable::getLocalTransform() const
{
	return transform;
}

//-----------------------------------//

} } // end namespaces