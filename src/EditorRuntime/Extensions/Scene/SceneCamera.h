/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Engine/Controllers/FirstPersonController.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

REFLECT_DECLARE_CLASS(SceneCameraController)

class SceneCameraController : public FirstPersonController
{
	REFLECT_DECLARE_OBJECT(SceneCameraController)

public:
 
	SceneCameraController();
};

//-----------------------------------//

NAMESPACE_EDITOR_END