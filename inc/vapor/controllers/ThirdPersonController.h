/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/controllers/CameraController.h"
#include "vapor/math/Vector2.h"
#include "vapor/math/EulerAngles.h"

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

	// Gets the follow node.
	GETTER(FollowNode, NodePtr, followNode.lock())

	// Sets the follow node.
	SETTER(FollowNode, const NodePtr&, followNode)

	// Updates this component.
	virtual void update( double delta );

protected:

	// Follow node name.
	std::string followNodeName;

	// Follow node.
	NodeWeakPtr followNode;

	// Distance to the tracked node.
	Vector3 distance;

	// Rotation to the tracked node.
	EulerAngles rotation;
};

TYPEDEF_SHARED_POINTER_FROM_TYPE( ThirdPersonController );

//-----------------------------------//

} // end namespace
