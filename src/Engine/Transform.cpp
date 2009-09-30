/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#include "vapor/scene/Transform.h"

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

void Transformable::rotate( float xang, float yang, float zang )
{

}

//-----------------------------------//

void Transformable::yaw( float ang )
{

}

//-----------------------------------//

void Transformable::pitch( float ang )
{

}

//-----------------------------------//

void Transformable::roll( float ang )
{

}

//-----------------------------------//

void Transformable::setAbsoluteLocalToWorld( const math::Matrix4& matrix )
{
	absoluteLocalToWorld = matrix;
}

//-----------------------------------//

const math::Matrix4& Transformable::getAbsoluteLocalToWorld() const
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