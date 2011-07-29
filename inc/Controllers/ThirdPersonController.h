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

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

/**
 * A simple camera controller that follows a node.
 */

REFLECT_DECLARE_CLASS(ThirdPersonController)

class API_ENGINE ThirdPersonController : public CameraController
{
	REFLECT_DECLARE_OBJECT(ThirdPersonController)

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

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( ThirdPersonController );

//-----------------------------------//

NAMESPACE_ENGINE_END
