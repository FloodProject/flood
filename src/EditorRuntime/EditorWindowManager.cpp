/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Editor/API.h"
#include "Editor/Editor.h"
#include "Editor/EditorWindowManager.h"
#include "Editor/RenderControl.h"
#include "Editor/RenderWindow.h"

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
    return control->getRenderWindow();
}

//-----------------------------------//