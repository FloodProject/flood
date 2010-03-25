/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/math/EulerAngles.h"
#include "vapor/math/AABB.h"
#include "vapor/scene/Component.h"

namespace vapor { namespace scene {

//-----------------------------------//

/**
 * Provides all scene entities with transform information. Transform are
 * used to scale, rotate and translate nodes in the scene. They will hold
 * all the needed matrices and will be updated by the physics manager.
 */

class VAPOR_API Transform : public Component
{
public:

	Transform( float x = 0.0f, float y = 0.0f, float z = 0.0f );

	// Translate this node by the given parameters.
	void translate( float x, float y, float z );
	void translate( const math::Vector3& tr );

	// Scale this node by the given parameters.
	void scale( float uniform );
	void scale( float x, float y, float z );
	void scale( const math::Vector3& scale );

	// Rotates this node by the given parameters.
	void rotate( float xang, float yang, float zang );
	void rotate( const math::Vector3& rot );

	// Resets all the transformations in the transform.
	void reset();

	// Gets the position of the node.
	IMPLEMENT_GETTER(Position, const math::Vector3&, translation)

	// Sets the position of the node.
	void setPosition( const math::Vector3& position );

	// Gets the rotation vector of this node.
	IMPLEMENT_GETTER(Rotation, const math::EulerAngles&, rotation)

	// Sets the rotation vector of this node.
	void setRotation( const math::EulerAngles& rot );

	// Points to a given point in space.
	math::Matrix4x3 lookAt( const math::Vector3& lookAtVector,
		const math::Vector3& upVector );

	// Gets the absolute transformation matrix.
	IMPLEMENT_GETTER(AbsoluteTransform, const math::Matrix4x3&, transform)
	
	// Sets the absolute transformation matrix.
	void setAbsoluteTransform( const math::Matrix4x3& matrix );

	// Gets the local transformation matrix.
	math::Matrix4x3 getLocalTransform() const;

	// Gets the bounding volume of the node.
	IMPLEMENT_GETTER(BoundingVolume, const math::AABB&, boundingVolume)

	// Gets the world bounding volume of the node.
	math::AABB getWorldBoundingVolume() const;

	// Updates the bounding volume geometry.
	void updateBoundingVolume();

	// Does this node's bounding box need to be updated?
	bool requiresBoundingVolumeUpdate() const;

	// Use this to render some debug bounding boxes.
	IMPLEMENT_GETTER(DebugRenderable, render::RenderablePtr, boundingVolumeRenderable)

	// Called once per frame to update the component.
	virtual void update( double delta );

	// Gets fired when the transform is changed.
	fd::delegate<void()> onTransform;

	// Gets the type of this component.
	IMPLEMENT_GETTER(Type, const std::string&, Transform::type)

	DECLARE_SERIALIZABLE();

protected:

	// Tracks if the transform has been changed.
	bool needsNotify;

	// Sends notifications when the transform has changed.
	void notify();

	math::Vector3 translation;
	math::EulerAngles rotation;
	math::Vector3 _scale;
	
	// Local transform
	math::Matrix4x3 transform;

	// Bounding volumes used for culling.
	bool needsVolumeUpdate;
	math::AABB boundingVolume;
	render::RenderablePtr boundingVolumeRenderable;

	// If an external update occurs (the matrix is changed) then we
	// don't want to override that when the transform is updated.
	bool externalUpdate;

	static const std::string& type;
};

//-----------------------------------//

TYPEDEF_SHARED_POINTER_FROM_TYPE( Transform );

//-----------------------------------//

} } // end namespaces