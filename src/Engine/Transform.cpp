/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/math/Math.h"
#include "vapor/scene/Node.h"
#include "vapor/scene/Transform.h"
#include "vapor/render/DebugGeometry.h"

using namespace vapor::math;
using namespace vapor::render;

namespace vapor { namespace scene {

//-----------------------------------//

const std::string& Transform::type = "Transform";

//-----------------------------------//

Transform::Transform( float x, float y, float z )
	: _scale( 1.0f ), translation( x, y, z ), needsNotify( false ),
	needsVolumeUpdate( true ), externalUpdate( false )
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
	needsNotify = true;

	translation.x += x;
	translation.y += y;
	translation.z += z;
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
	needsNotify = true;

	_scale.x *= x;
	_scale.y *= y;
	_scale.z *= z;
}

//-----------------------------------//

void Transform::rotate( const math::Vector3& rot )
{
	rotate( rot.x, rot.y, rot.z );
}

//-----------------------------------//

void Transform::rotate( float xang, float yang, float zang )
{
	needsNotify = true;

	rotation.x += xang;
	rotation.y += yang;
	rotation.z += zang;
}

//-----------------------------------//

void Transform::setRotation( const math::EulerAngles& rot )
{
	needsNotify = true;
	rotation = rot;
}

//-----------------------------------//

void Transform::setPosition( const math::Vector3& position )
{
	needsNotify = true;
	translation = position;
}

//-----------------------------------//

Matrix4x3 Transform::lookAt( const Vector3& lookAtVector, const Vector3& upVector )
{
	const Vector3& eye = translation;

	Vector3 zaxis = (eye - lookAtVector).normalize();
	Vector3	xaxis = upVector.cross(zaxis).normalize();
	Vector3	yaxis = zaxis.cross(xaxis);

	Matrix4x3 m;
	m.m11 = xaxis.x;
	m.m12 = yaxis.x;
	m.m13 = zaxis.x;

	m.m21 = xaxis.y;
	m.m22 = yaxis.y;
	m.m23 = zaxis.y;

	m.m31 = xaxis.z;
	m.m32 = yaxis.z;
	m.m33 = zaxis.z;

	m.tx = -xaxis.dot(eye);
	m.ty = -yaxis.dot(eye);
	m.tz = -zaxis.dot(eye);

	return m;
}

//-----------------------------------//

void Transform::reset( )
{
	needsNotify = true;

	translation.zero();
	_scale = math::Vector3( 1.0f );
	rotation.identity();
}

//-----------------------------------//

void Transform::setAbsoluteTransform( const math::Matrix4x3& matrix )
{
	needsNotify = true;
	externalUpdate = true;
	transform = matrix;
}

//-----------------------------------//

math::Matrix4x3 Transform::getLocalTransform() const
{
	return Matrix4x3::createTranslationMatrix( translation )
		* rotation.getOrientationMatrix()
		* Matrix4x3::createScaleMatrix( _scale );
}

//-----------------------------------//

void Transform::markBoundingVolumeDirty()
{
	needsVolumeUpdate = true;
}

//-----------------------------------//

bool Transform::requiresBoundingVolumeUpdate() const
{
	return needsVolumeUpdate;
}

//-----------------------------------//

void Transform::updateBoundingVolume()
{
	const NodePtr& node = getNode();
	if( !node ) return;

	boundingVolume.reset();

	foreach( const GeometryPtr& geometry, node->getGeometry() )
	{
		boundingVolume.add( geometry->getBoundingVolume() );
	}
	
	// Update debug renderable.
	boundingVolumeRenderable = buildBoundingRenderable( boundingVolume );
	needsVolumeUpdate = false;
}

//-----------------------------------//

void Transform::update( double VAPOR_UNUSED(delta) )
{
	if( !externalUpdate )
		transform = getLocalTransform();

	if( requiresBoundingVolumeUpdate() )
		updateBoundingVolume();

	if( needsNotify )
	{
		notify();
		needsNotify = false;
	}

	externalUpdate = false;
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

math::AABB Transform::getWorldBoundingVolume() const
{
	return boundingVolume.transform( getAbsoluteTransform() );
}

//-----------------------------------//

void Transform::serialize( Json::Value& value )
{
	value["position"] = toJson(translation);
	value["rotation"] = toJson(rotation);
	value["scale"] = toJson(_scale);
}

//-----------------------------------//

} } // end namespaces