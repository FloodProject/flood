/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "controllers/CameraController.h"
#include "Math/Vector.h"
#include "Math/EulerAngles.h"

namespace vapor {

//-----------------------------------//

/**
 * A simple camera controller that follows a node.
 */

class VAPOR_API ThirdPersonController : public CameraController
{
	DECLARE_CLASS_()

public:

	ThirdPersonController();

	// Gets/sets the follow node.
	ACESSOR(FollowEntity, EntityPtr, followEntity)

	// Updates this component.
	virtual void _update( float delta );

protected:

	// Follow node name.
	std::string followEntityName;

	// Follow node.
	EntityPtr followEntity;

	// Distance to the tracked node.
	Vector3 distance;

	// Rotation to the tracked node.
	EulerAngles rotation;
};

TYPEDEF_SHARED_POINTER_FROM_TYPE( ThirdPersonController );

//-----------------------------------//

} // end namespace
