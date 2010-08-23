/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/math/EulerAngles.h"
#include "vapor/math/BoundingBox.h"
#include "vapor/scene/Component.h"

namespace vapor {

//-----------------------------------//

/**
 * Provides all scene entities with transform information. Transform are
 * used to scale, rotate and translate nodes in the scene. They will hold
 * all the needed matrices and will be updated by the physics manager.
 */

class VAPOR_API Transform : public Component
{
	DECLARE_CLASS_()

public:

	Transform( float x = 0.0f, float y = 0.0f, float z = 0.0f );
	Transform( const Transform& rhs );
	~Transform();

	// Translate this node by the given parameters.
	void translate( float x, float y, float z );
	void translate( const Vector3& tr );

	// Scale this node by the given parameters.
	void scale( float uniform );
	void scale( float x, float y, float z );
	void scale( const Vector3& scale );

	// Rotates this node by the given parameters.
	void rotate( float xang, float yang, float zang );
	void rotate( const Vector3& rot );

	// Resets all the transformations in the transform.
	void reset();

	// Gets the position of the transform.
	GETTER(Position, const Vector3&, translation)

	// Sets the position of the transform.
	void setPosition( const Vector3& position );

	// Gets the scale of the transform.
	GETTER(Scale, const Vector3&, scaling)

	// Sets the scale of the transform.
	void setScale( const Vector3& scale );

	// Gets the rotation vector of the transform.
	GETTER(Rotation, const EulerAngles&, rotation)

	// Sets the rotation vector of the transform.
	void setRotation( const EulerAngles& rot );

	// Points to a given point in space.
	Matrix4x3 lookAt( const Vector3& lookAtVector, const Vector3& upVector );

	// Gets the absolute transformation matrix.
	GETTER(AbsoluteTransform, const Matrix4x3, getLocalTransform())
	
	// Sets the absolute transformation matrix.
	void setAbsoluteTransform( const Matrix4x3& matrix );

	// Gets the local transformation matrix.
	Matrix4x3 getLocalTransform() const;

	// Gets the position of the transform in world space.
	Vector3 getWorldPosition() const;

	// Gets the bounding volume of the transform.
	GETTER(BoundingVolume, const AABB&, boundingVolume)

	// Gets the world bounding volume of the transform.
	AABB getWorldBoundingVolume() const;

	// Updates the bounding volume geometry.
	void updateBoundingVolume();

	// Marks the bounding volume as needing an update.
	void markBoundingVolumeDirty();

	// Gets if the bounding volume need to be updated.
	bool requiresBoundingVolumeUpdate() const;

	// Use this to render some debug bounding boxes.
	GETTER(DebugRenderable, RenderablePtr, boundingVolumeRenderable)

	// Sets the notify bit of the transform.
	void setNotify();

	// Unsets the notify bit of the transform.
	void unsetNotify();

	// Called once per frame to update the component.
	virtual void update( double delta );

	// Gets fired when the transform is changed.
	fd::delegate<void()> onTransform;

protected:

	// Handles field changes notifications.
	void onFieldChanged(const Field& field);

	// Tracks if the transform has been changed.
	bool needsNotify;

	// Sends notifications when the transform has changed.
	void notify();

	// Translation.
	Vector3 translation;

	// Orientation.
	EulerAngles rotation;

	// Scaling.
	Vector3 scaling;
	
	// Local transform.
	Matrix4x3 transform;

	// Does the bounding volume needs to be rebuilt.
	bool needsVolumeUpdate;

	// Bounding volume of the renderables.
	AABB boundingVolume;

	// Geometry of the bounding volume.
	RenderablePtr boundingVolumeRenderable;

	// If an external transform is given, don't generate our own.
	bool externalTransform;
};

TYPEDEF_SHARED_POINTER_FROM_TYPE( Transform );

//-----------------------------------//

} // end namespace