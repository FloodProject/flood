/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
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