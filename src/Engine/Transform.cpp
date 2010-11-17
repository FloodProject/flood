/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "scene/Transform.h"
#include "scene/Node.h"
#include "render/DebugGeometry.h"
#include "math/Math.h"

namespace vapor {

//-----------------------------------//

BEGIN_CLASS_PARENT(Transform, Component)
	FIELD_PRIMITIVE(Transform, Vector3, position)
	FIELD_PRIMITIVE(Transform, Quaternion, rotation)
	FIELD_PRIMITIVE(Transform, Vector3, scaling)
END_CLASS()

//-----------------------------------//

Transform::Transform( float x, float y, float z )
	: position( x, y, z )
	, scaling( 1.0f )
{
	init();
}

//-----------------------------------//

Transform::Transform( const Transform& rhs )
	: position( rhs.position )
	, rotation( rhs.rotation )
	, scaling( rhs.scaling )
{
	init();
}

//-----------------------------------//

Transform::~Transform()
{
	Class& type = getType();
	type.onFieldChanged.Disconnect( this, &Transform::onFieldChanged );
}

//-----------------------------------//

void Transform::init()
{
	needsNotify = false;
	needsVolumeUpdate = true;
	externalTransform = false;

	Class& type = getType();
	type.onFieldChanged.Connect( this, &Transform::onFieldChanged );
}

//-----------------------------------//

void Transform::translate( const Vector3& offset )
{
	translate( offset.x, offset.y, offset.z );
}

//-----------------------------------//

void Transform::translate( float x, float y, float z )
{
	setNotify();

	position.x += x;
	position.y += y;
	position.z += z;
}

//-----------------------------------//

void Transform::scale( float value )
{
	scale( value, value, value );
}

//-----------------------------------//

void Transform::scale( const Vector3& value )
{
	scale( value.x, value.y, value.z );
}

//-----------------------------------//

void Transform::scale( float x, float y, float z )
{
	setNotify();

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
	setNotify();

	rotation.x += xang;
	rotation.y += yang;
	rotation.z += zang;
}

//-----------------------------------//

void Transform::setPosition( const Vector3& newTranslation )
{
	setNotify();
	position = newTranslation;
}

//-----------------------------------//

void Transform::setRotation( const Quaternion& newRotation )
{
	setNotify();
	rotation = newRotation;
}

//-----------------------------------//

void Transform::setScale( const Vector3& newScale )
{
	setNotify();
	scaling = newScale;
}

//-----------------------------------//

Vector3 Transform::getWorldPosition() const
{
	return getAbsoluteTransform()*getPosition();
}

//-----------------------------------//

Matrix4x3 Transform::lookAt( const Vector3& lookAtVector, const Vector3& upVector )
{
	const Vector3& eye = position;

	Vector3 zaxis = (eye - lookAtVector).normalize();
	Vector3	xaxis = upVector.cross(zaxis).normalize();
	Vector3	yaxis = zaxis.cross(xaxis);

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

void Transform::reset( )
{
	setNotify();

	position.zero();
	rotation.identity();
	scaling = Vector3( 1.0f );
}

//-----------------------------------//

void Transform::setNotify(bool state)
{
	needsNotify = state;
}

//-----------------------------------//

const Matrix4x3& Transform::getAbsoluteTransform() const
{
	return transform;
}

//-----------------------------------//

void Transform::setAbsoluteTransform( const Matrix4x3& newTransform )
{
	transform = newTransform;
	externalTransform = true;
	setNotify();
}

//-----------------------------------//

Matrix4x3 Transform::getLocalTransform() const
{
	Matrix4x3 matScale = Matrix4x3::createScale(scaling);
	Matrix4x3 matRotation = Matrix4x3::createFromQuaternion(rotation);
	Matrix4x3 matTranslation = Matrix4x3::createTranslation(position);
	
	Matrix4x3 matTransform = matScale*matRotation*matTranslation;
	return matTransform;
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
	
	if( !node )
		return;

	boundingVolume.reset();

	foreach( const GeometryPtr& geometry, node->getGeometry() )
		boundingVolume.add( geometry->getBoundingVolume() );
	
	if( boundingVolume.isInfinite() )
		boundingVolume.setZero();

	// Update debug renderable.
	debugRenderable = buildBoundingRenderable( boundingVolume );
	needsVolumeUpdate = false;
}

//-----------------------------------//

void Transform::update( double delta )
{
	if( !externalTransform )
		transform = getLocalTransform();

	if( requiresBoundingVolumeUpdate() )
		updateBoundingVolume();

	if( needsNotify )
	{
		sendNotifications();
		setNotify(false);
	}

	externalTransform = false;
}

//-----------------------------------//

void Transform::sendNotifications()
{
	onTransform();
}

//-----------------------------------//

BoundingBox Transform::getWorldBoundingVolume() const
{
	const Matrix4x3& transform = getAbsoluteTransform();
	return boundingVolume.transform(transform);
}

//-----------------------------------//

void Transform::onFieldChanged(const Field& field)
{
	setNotify();
}

//-----------------------------------//

} // end namespace