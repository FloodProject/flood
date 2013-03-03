/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Editor/API.h"
#include "Engine/Window/WindowManager.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

class EditorWindowManager : public WindowManager
{
public:

    EditorWindowManager();

    Window* createWindow(const WindowSettings& settings) OVERRIDE;
};

//-----------------------------------//

NAMESPACE_EDITOR_END