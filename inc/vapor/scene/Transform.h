/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/scene/Component.h"
#include "vapor/math/EulerAngles.h"
#include "vapor/math/AABB.h"

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
	virtual ~Transform();

	// Translate this node by the given parameters.
	void translate( float x, float y, float z );
	void translate( const math::Vector3& tr );

	// Gets the position of the node.
	const math::Vector3& getPosition() const;

	// Sets the position of the node.
	void setPosition( const math::Vector3& position );

	// Scale this node by the given parameters.
	void scale( float uniform );
	void scale( float x, float y, float z );
	void scale( const math::Vector3& scale );

	// Rotates this node by the given parameters.
	void rotate( float xang, float yang, float zang );
	void rotate( const math::Vector3& rot );

	// Gets the rotation vector of this node.
	const math::EulerAngles& getRotation() const;

	// Sets the rotation vector of this node.
	void setRotation( math::EulerAngles& rot );

	// Points to a given point in space.
	math::Matrix4x3 lookAt( const math::Vector3& lookAtVector, const math::Vector3& upVector );
	
	// Resets all the transformations in the transform.
	void reset();

	// Gets the absolute transformation matrix.
	const math::Matrix4x3& getAbsoluteTransform() const;
	
	// Sets the absolute transformation matrix.
	void setAbsoluteTransform( const math::Matrix4x3& matrix );

	// Gets the local transformation matrix.
	math::Matrix4x3 getLocalTransform() const;

	// Gets the bounding volume of the node.
	const math::AABB& getBoundingVolume() const;

	// Gets the world bounding volume of the node.
	const math::AABB& getWorldBoundingVolume() const;

	// Does this node's bounding box need to be updated?
	bool requiresBoundingVolumeUpdate() const;

	// Called once per frame to update the component.
	virtual void update( double delta );

	// Gets fired when the transform is changed.
	fd::delegate< void() > onTransform;

	// Gets the type of this component. 
	virtual const std::string& getType() const;

	// Use this to render some debug bounding boxes.
	virtual render::RenderablePtr getDebugRenderable() const;

protected:

	void notify();

	math::EulerAngles angles;
	math::Vector3 v_translate;
	math::Vector3 v_scale;

	math::Matrix4x3 transform;
	math::Matrix4x3 absoluteLocalToWorld;

	// Bounding volumes used for culling.
	math::AABB boundingVolume;
	math::AABB worldBoundingVolume;

	bool aabbNeedsUpdate;
	render::RenderablePtr aabbRenderable;

	static const std::string& type;

public:

	bool isPhysicsDriven;
};

//-----------------------------------//

TYPEDEF_SHARED_POINTER_FROM_CLASS( Transform );

//-----------------------------------//

} } // end namespaces