/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Editor/API.h"
#include "SceneCamera.h"

//-----------------------------------//

REFLECT_CHILD_CLASS(SceneCameraController, FirstPersonController)
REFLECT_CLASS_END()

//-----------------------------------//

SceneCameraController::SceneCameraController() : FirstPersonController()
{
}

//-----------------------------------//

NAMESPACE_EDITOR_END