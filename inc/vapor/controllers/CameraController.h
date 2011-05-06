/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "controllers/Controller.h"

namespace vapor {

//-----------------------------------//

/**
 * A simple camera controller.
 */

REFLECT_DECLARE_CLASS(CameraController)

class VAPOR_API CameraController : public Controller
{
	REFLECT_DECLARE_OBJECT(CameraController)

public:

	// Updates this component.
	virtual void _update( float delta ) = 0;

	// Gets/sets the look sensivity of the camera.
	ACESSOR(LookSensivity, float, lookSensivity);
	
	// Gets/sets the move sensivity of the camera.
	ACESSOR(MoveSensivity, float, moveSensivity);

protected:

	CameraController();

	// Camera sensivity.
	float lookSensivity;
	float moveSensivity;
};

TYPEDEF_SHARED_POINTER_FROM_TYPE( CameraController );

//-----------------------------------//

} // end namespace
