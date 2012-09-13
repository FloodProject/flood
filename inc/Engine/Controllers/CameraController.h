/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Engine/Controllers/Controller.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

/**
 * A simple camera controller.
 */

API_ENGINE REFLECT_DECLARE_CLASS(CameraController)

class API_ENGINE CameraController : public Controller
{
	REFLECT_DECLARE_OBJECT(CameraController)

public:

	// Updates this component.
	virtual void _update( float delta ) OVERRIDE = 0;

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

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( CameraController );

//-----------------------------------//

NAMESPACE_ENGINE_END
