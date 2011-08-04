/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"
#include "Controllers/CameraController.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

REFLECT_ABSTRACT_CHILD_CLASS(CameraController, Controller)
	FIELD_PRIMITIVE(7, float, lookSensivity)
	FIELD_PRIMITIVE(8, float, moveSensivity)
REFLECT_CLASS_END()

//-----------------------------------//

static const float DEFAULT_MOVE_SENSIVITY = 50.0f;
static const float DEFAULT_LOOK_SENSIVITY = 20.0f;

//-----------------------------------//

CameraController::CameraController()
	: moveSensivity( DEFAULT_MOVE_SENSIVITY )
	, lookSensivity( DEFAULT_LOOK_SENSIVITY )
{ }

//-----------------------------------//

NAMESPACE_ENGINE_END