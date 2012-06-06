/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
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