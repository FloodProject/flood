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

namespace vapor {

//-----------------------------------//

BEGIN_CLASS_PARENT(Transform, Component)
	FIELD_PRIMITIVE(Transform, Vector3, translation)
	FIELD_PRIMITIVE(Transform, Quaternion, rotation)
	FIELD_PRIMITIVE(Transform, Vector3, scaling)
END_CLASS()

//-----------------------------------//

Transform::Transform( float x, float y, float z )
	: scaling( 1.0f )
	, translation( x, y, z )
	, needsNotify( false )
	, needsVolumeUpdate( true )
	, externalTransform( false )
{
	Class& klass = getType();
	klass.onFieldChanged += fd::bind(&Transform::onFieldChanged, this);
}

//-----------------------------------//

Transform::Transform( const Transform& rhs )
	: scaling( rhs.scaling )
	, translation( rhs.translation )
	, rotation( rhs.rotation )
	, needsVolumeUpdate( false )
	, needsNotify( false )
	, externalTransform( false )
{
	Class& klass = getType();
	klass.onFieldChanged += fd::bind(&Transform::onFieldChanged, this);
}

//-----------------------------------//

Transform::~Transform()
{
	Class& klass = getType();
	klass.onFieldChanged -= fd::bind(&Transform::onFieldChanged, this);
}

//-----------------------------------//

void Transform::translate( const Vector3& tr )
{
	translate( tr.x, tr.y, tr.z );
}

//-----------------------------------//

void Transform::translate( float x, float y, float z )
{
	setNotify();

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

void Transform::scale( const Vector3& s )
{
	scale( s.x, s.y, s.z );
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
	translation = newTranslation;
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
	return getPosition()*getAbsoluteTransform();
}

//-----------------------------------//

Matrix4x3 Transform::lookAt( const Vector3& lookAtVector, const Vector3& upVector )
{
	const Vector3& eye = translation;

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

	translation.zero();
	scaling = Vector3( 1.0f );
	rotation.identity();
}

//-----------------------------------//

void Transform::setNotify()
{
	needsNotify = true;
}

//-----------------------------------//

void Transform::unsetNotify()
{
	needsNotify = false;
}

//-----------------------------------//

void Transform::setAbsoluteTransform( const Matrix4x3& newTransform )
{
	setNotify();
	externalTransform = true;
	transform = newTransform;
}

//-----------------------------------//

Matrix4x3 Transform::getLocalTransform() const
{
	Matrix4x3 matScale = Matrix4x3::createScale(scaling);
	Matrix4x3 matRotation = Matrix4x3::createFromQuaternion(rotation);
	Matrix4x3 matTranslation = Matrix4x3::createTranslation(translation);
	
	// Combine all the transformations in a single matrix.
	Matrix4x3 transform = matScale*matRotation*matTranslation;

	return transform;
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
	if( !externalTransform )
		transform = getLocalTransform();

	if( requiresBoundingVolumeUpdate() )
		updateBoundingVolume();

	if( needsNotify )
	{
		notify();
		unsetNotify();
	}

	externalTransform = false;
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

AABB Transform::getWorldBoundingVolume() const
{
	return boundingVolume.transform( getAbsoluteTransform() );
}

//-----------------------------------//

void Transform::onFieldChanged(const Field& field)
{
	setNotify();
}

//-----------------------------------//

} // end namespace