/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "scene/Transform.h"
#include "scene/Geometry.h"
#include "scene/Entity.h"

#include "math/Math.h"
#include "render/DebugGeometry.h"

namespace vapor {

//-----------------------------------//

BEGIN_CLASS_PARENT(Transform, Component)
	FIELD_PRIMITIVE(Transform, Vector3, position)
	FIELD_PRIMITIVE(Transform, Quaternion, rotation)
	FIELD_PRIMITIVE(Transform, Vector3, scaling)
END_CLASS()

//-----------------------------------//

Transform::Transform()
	: wasChanged(false)
	, needsBoundsUpdate(true)
	, externalTransform(false)
	, scaling(1)
{ }

//-----------------------------------//

void Transform::translate( const Vector3& offset )
{
	translate( offset.x, offset.y, offset.z );
}

//-----------------------------------//

void Transform::translate( float x, float y, float z )
{
	setChanged();

	position.x += x;
	position.y += y;
	position.z += z;
}

//-----------------------------------//

void Transform::scale( const Vector3& value )
{
	scale( value.x, value.y, value.z );
}

//-----------------------------------//

void Transform::scale( float x, float y, float z )
{
	setChanged();

	scaling.x *= x;
	scaling.y *= y;
	scaling.z *= z;
}

//-----------------------------------//

void Transform::rotate( const Vector3& rot )
{
	rotate( rot.x, rot.y, rot.z );
}

//-----------------------------------//

void Transform::rotate( float xang, float yang, float zang )
{
	setChanged();

	rotation.x += xang;
	rotation.y += yang;
	rotation.z += zang;
}

//-----------------------------------//

void Transform::setPosition( const Vector3& newTranslation )
{
	setChanged();
	position = newTranslation;
}

//-----------------------------------//

void Transform::setRotation( const Quaternion& newRotation )
{
	setChanged();
	rotation = newRotation;
}

//-----------------------------------//

void Transform::setScale( const Vector3& newScale )
{
	setChanged();
	scaling = newScale;
}

//-----------------------------------//

void Transform::reset()
{
	setChanged();

	position.zero();
	rotation.identity();
	scaling = 1;
}

//-----------------------------------//

Vector3 Transform::getWorldPosition() const
{
	return getAbsoluteTransform() * getPosition();
}

//-----------------------------------//

Matrix4x3 Transform::lookAt( const Vector3& lookAtVector, const Vector3& upVector )
{
	const Vector3& eye = position;

	Vector3 zaxis = (eye - lookAtVector).normalize();
	Vector3 xaxis = upVector.cross(zaxis).normalize();
	Vector3 yaxis = zaxis.cross(xaxis);

	Matrix4x3 mat;
	mat.m11 = xaxis.x;
	mat.m12 = yaxis.x;
	mat.m13 = zaxis.x;

	mat.m21 = xaxis.y;
	mat.m22 = yaxis.y;
	mat.m23 = zaxis.y;

	mat.m31 = xaxis.z;
	mat.m32 = yaxis.z;
	mat.m33 = zaxis.z;

	mat.tx = -xaxis.dot(eye);
	mat.ty = -yaxis.dot(eye);
	mat.tz = -zaxis.dot(eye);

	return mat;
}

//-----------------------------------//

void Transform::setChanged(bool state)
{
	wasChanged = state;
}

//-----------------------------------//

const Matrix4x3& Transform::getAbsoluteTransform() const
{
	return transform;
}

//-----------------------------------//

void Transform::setAbsoluteTransform( const Matrix4x3& newTransform )
{
	setChanged();

	transform = newTransform;
	externalTransform = true;
}

//-----------------------------------//

Matrix4x3 Transform::getLocalTransform() const
{
	Matrix4x3 matScale = Matrix4x3::createScale(scaling);
	Matrix4x3 matRotation = Matrix4x3::createFromQuaternion(rotation);
	Matrix4x3 matTranslation = Matrix4x3::createTranslation(position);
	
	return matScale*matRotation*matTranslation;
}

//-----------------------------------//

void Transform::markBoundingVolumeDirty()
{
	needsBoundsUpdate = true;
}

//-----------------------------------//

bool Transform::requiresBoundingVolumeUpdate() const
{
	return needsBoundsUpdate;
}

//-----------------------------------//

void Transform::updateBoundingVolume()
{
	const EntityPtr& node = getEntity();
	
	if( !node )
		return;

	bounds.reset();

	for( uint i = 0; i < node->getGeometry().size(); i++ )
	{
		const GeometryPtr& geometry = node->getGeometry()[i];
		bounds.add( geometry->getBoundingVolume() );
	}
	
	if( bounds.isInfinite() )
		bounds.setZero();

	// Update debug renderable.
	debugRenderable = buildBoundingRenderable( bounds );
	needsBoundsUpdate = false;
}

//-----------------------------------//

void Transform::update( double delta )
{
	if( !externalTransform )
		transform = getLocalTransform();

	if( requiresBoundingVolumeUpdate() )
		updateBoundingVolume();

	if( wasChanged )
	{
		onTransform();
		setChanged(false);
	}

	//externalTransform = false;
}

//-----------------------------------//

BoundingBox Transform::getWorldBoundingVolume() const
{
	const Matrix4x3& transform = getAbsoluteTransform();
	return bounds.transform(transform);
}

//-----------------------------------//

} // end namespace