/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"

#include "vapor/scene/Component.h"

#include "vapor/math/Matrix4x3.h"
#include "vapor/math/Vector3.h"
#include "vapor/math/EulerAngles.h"

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

	Transform();
	virtual ~Transform();

	// Translate this node by the given parameters.
	void translate( float x, float y, float z );
	void translate( const math::Vector3& tr );

	// Gets the position of the node.
	const math::Vector3& getPosition() const;

	// Sets the position of the node.
	void setPosition( const math::Vector3 position );

	// Scale this node by the given parameters.
	void scale( float x, float y, float z );
	void scale( float uniform );
	void scale( const math::Vector3& scale );

	// Rotates this node by the given parameters.
	void rotate( float xang, float yang, float zang );
	void rotate( const math::Vector3& rot );

	// Gets the rotation vector of this node.
	const math::EulerAngles& getRotation() const;

	// Sets the rotation vector of this node.
	void setRotation( math::EulerAngles& rot );

	// Points to a given point in space.
	void lookAt( const math::Vector3& lookAtVector, const math::Vector3& upVector );
	
	// Resets the transformations previously applied to this node.
	void reset();

	// Gets the absolute transformation matrix.
	const math::Matrix4x3& getAbsoluteTransform() const;
	
	// Sets the absolute transformation matrix.
	void setAbsoluteTransform( const math::Matrix4x3& matrix );

	// Gets the local transformation matrix.
	math::Matrix4x3 getLocalTransform() const;

	// Gets the type of this component. 
	virtual const std::string& getType() const;

	// Called once per frame to update the component.
	virtual void update( float delta );

	fd::delegate< void() > onTransform;

protected:

	void notify();

	math::EulerAngles angles;
	math::Vector3 v_translate;
	math::Vector3 v_scale;

	math::Matrix4x3 transform;
	math::Matrix4x3 absoluteLocalToWorld;

	static const std::string& type;
};

//-----------------------------------//

TYPEDEF_SHARED_POINTER_FROM_CLASS( Transform );

//-----------------------------------//

} } // end namespaces