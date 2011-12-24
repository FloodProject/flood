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

REFLECT_CHILD_CLASS(SceneCamera, FirstPersonController)
REFLECT_CLASS_END()

//-----------------------------------//

SceneCamera::SceneCamera() : FirstPersonController()
{
}

//-----------------------------------//

NAMESPACE_EDITOR_END