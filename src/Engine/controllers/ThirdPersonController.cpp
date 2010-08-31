/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/controllers/ThirdPersonController.h"

#include "vapor/Engine.h"
#include "vapor/math/Math.h"
#include "vapor/scene/Node.h"
#include "vapor/scene/Camera.h"
#include "vapor/render/Device.h"
#include "vapor/input/InputManager.h"
#include "vapor/input/Keyboard.h"
#include "vapor/input/Mouse.h"

namespace vapor {

//-----------------------------------//

BEGIN_CLASS_PARENT(ThirdPersonController, CameraController)
END_CLASS()

//-----------------------------------//

ThirdPersonController::ThirdPersonController()
{ }

//-----------------------------------//

void ThirdPersonController::update( double delta )
{
	if( !enabled )
		return;
//	const TransformPtr& transform = getNode()->getTransform();
//
//	Vector3 thirdPersonReference = new Vector3(0, 200, -200);
//	Matrix rotationMatrix = Matrix.CreateRotationY(avatarYaw);
//
//	// Create a vector pointing the direction the camera is facing.
//	Vector3 transformedReference = Vector3.Transform(thirdPersonReference, rotationMatrix);
//
//	// Calculate the position the camera is looking from.
//	Vector3 cameraPosition = transformedReference + avatarPosition;
//
//	view = Matrix.CreateLookAt(cameraPosition, avatarPosition, 
//	    new Vector3(0.0f, 1.0f, 0.0f));
}

//-----------------------------------//

} // end namespace