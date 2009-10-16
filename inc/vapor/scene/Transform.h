/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"

#include "vapor/scene/Node.h"

#include "vapor/math/Matrix4x3.h"
#include "vapor/math/Vector3.h"
#include "vapor/math/EulerAngles.h"

namespace vapor {
	namespace scene {

//-----------------------------------//

class VAPOR_API Transformable : public Node
{
public:

	// Translate this node by the given parameters.
	void translate( const math::Vector3& tr );

	// Scale this node by the given parameters.
	void scale( float x, float y, float z );
	void scale( float uniform );

	// Rotates this node by the given parameters.
	void rotate( float xang, float yang, float zang );

	// Resets the transformations previously applied to this node.
	void reset();

	// Gets/sets the abolute
	const math::Matrix4x3& getAbsoluteTransform() const;
	void setAbsoluteTransform( const math::Matrix4x3& matrix );

	// Gets the local transformation matrix.
	math::Matrix4x3 getLocalTransform() const;

protected:

	Transformable();
	~Transformable();

	math::Matrix4x3 getScaleMatrix( math::Vector3 v ) const;
	math::Matrix4x3 getTranslationMatrix( math::Vector3 v ) const;

	math::EulerAngles angles;
	math::Vector3 v_translate;
	math::Vector3 v_scale;
	math::Matrix4x3 transform;

	math::Matrix4x3 absoluteLocalToWorld;
};

//-----------------------------------//

typedef tr1::shared_ptr< Transformable > TransformablePtr;

//-----------------------------------//

} } // end namespaces