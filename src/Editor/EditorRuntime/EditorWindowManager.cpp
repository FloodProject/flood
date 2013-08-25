/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "EditorRuntime/API.h"
#include "Editor.h"
#include "EditorWindowManager.h"
#include "RenderControl.h"
#include "RenderWindow.h"

//-----------------------------------//

EditorWindowManager::EditorWindowManager()
{
}

//-----------------------------------//

Window* EditorWindowManager::createWindow(const WindowSettings& settings)
{
    wxWindow* parent = &GetEditor();

    if ((int)settings.styles & (int)WindowStyles::TopLevel)
    {
        parent = new wxFrame(nullptr, wxID_ANY, settings.title);
        parent->Show();
    }

    RenderControl* control = new RenderControl(parent);
    control->Show();

    return control->getRenderWindow();
}

//-----------------------------------//