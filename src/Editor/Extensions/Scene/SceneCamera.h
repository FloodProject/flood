/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Controllers/FirstPersonController.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

REFLECT_DECLARE_CLASS(SceneCamera)

class SceneCamera : public FirstPersonController
{
	REFLECT_DECLARE_OBJECT(SceneCamera)

public:
 
	SceneCamera();
};

//-----------------------------------//

NAMESPACE_EDITOR_END