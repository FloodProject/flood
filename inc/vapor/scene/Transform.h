/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "math/EulerAngles.h"
#include "math/Quaternion.h"
#include "math/BoundingBox.h"
#include "scene/Component.h"

namespace vapor {

//-----------------------------------//

/**
 * Transform provide entities with position and orientation information.
 * They can be used to scale, rotate and translate entities in the scene,
 * and keep track of their bounding volumes aswell.
 */

class VAPOR_API Transform : public Component
{
	DECLARE_CLASS_()
	DECLARE_UNCOPYABLE(Transform)

public:

	Transform();

	// Translates by the given parameters.
	void translate( float x, float y, float z );
	void translate( const Vector3& tr );

	// Scales by the given parameters.
	void scale( float x, float y, float z );
	void scale( const Vector3& scale );

	// Rotates by the given parameters.
	void rotate( float xang, float yang, float zang );
	void rotate( const Vector3& rotation );

	// Resets all the transformations.
	void reset();

	// Gets the position of the transform.
	GETTER(Position, const Vector3&, position)

	// Sets the position of the transform.
	void setPosition( const Vector3& position );

	// Gets the scale of the transform.
	GETTER(Scale, const Vector3&, scaling)

	// Sets the scale of the transform.
	void setScale( const Vector3& scale );

	// Gets the rotation quaternion of the transform.
	GETTER(Rotation, const Quaternion&, rotation)

	// Gets the rotation matrix of the transform.
	GETTER(RotationMatrix, Matrix4x3, Matrix4x3::createFromQuaternion(rotation))

	// Sets the rotation vector of the transform.
	void setRotation( const Quaternion& rotation );

	// Points to a given point in space.
	Matrix4x3 lookAt( const Vector3& lookAtVector, const Vector3& upVector );

	// Gets the absolute transformation matrix.
	const Matrix4x3& getAbsoluteTransform() const;
	
	// Sets the absolute transformation matrix.
	void setAbsoluteTransform( const Matrix4x3& matrix );

	// Gets the local transformation matrix.
	Matrix4x3 getLocalTransform() const;

	// Gets the position of the transform in world space.
	Vector3 getWorldPosition() const;

	// Gets the bounding volume of the transform.
	GETTER(BoundingVolume, const BoundingBox&, bounds)

	// Gets the world bounding volume of the transform.
	BoundingBox getWorldBoundingVolume() const;

	// Updates the bounding volume geometry.
	void updateBoundingVolume();

	// Marks the bounding volume as needing an update.
	void markBoundingVolumeDirty();

	// Gets if the bounding volume need to be updated.
	bool requiresBoundingVolumeUpdate() const;

	// Called once per frame to update the component.
	virtual void update( double delta );

	// Gets fired when the transform is changed.
	Event0<> onTransform;

protected:

	// Sets the notify state of the transform.
	void setChanged(bool state = true);

	// Position.
	Vector3 position;

	// Orientation.
	Quaternion rotation;

	// Scaling.
	Vector3 scaling;
	
	// Local transform.
	Matrix4x3 transform;

	// Bounding volume of the renderables.
	BoundingBox bounds;

	// Tracks if the transform has been changed.
	bool wasChanged;

	// Does the bounding volume needs to be rebuilt.
	bool needsBoundsUpdate;

	// If an external transform is given, don't generate our own.
	bool externalTransform;
};

TYPEDEF_SHARED_POINTER_FROM_TYPE( Transform );

//-----------------------------------//

} // end namespace