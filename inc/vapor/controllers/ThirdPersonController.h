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
 * A simple First-Person style camera controller.
 */

class VAPOR_API ThirdPersonController : public CameraController
{
	DECLARE_CLASS_()

public:

	ThirdPersonController();

	// Updates this component.
	virtual void update( double delta );

protected:

	// Relative movement position.
	Vector3 relativePosition;
	EulerAngles rotation;
};

TYPEDEF_SHARED_POINTER_FROM_TYPE( ThirdPersonController );

//-----------------------------------//

} // end namespace
