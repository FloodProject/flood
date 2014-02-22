/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "WxPlatform.h"
#include "WxRenderContext.h"
#include "WxWindow.h"
#include "WxMenu.h"
#include <wx/menu.h>

//-----------------------------------//

WxWindowManager::WxWindowManager(WxPlatform* platform)
    : platform(platform)
{
}

//-----------------------------------//

static long ConvertToWxStyles(WindowStyles styles)
{
    return wxDEFAULT_FRAME_STYLE;
}

Window* WxWindowManager::createWindow(const WindowSettings& settings)
{
    wxWindow* nativeWindow = 0;

    if ((int)settings.styles & (int)WindowStyles::TopLevel)
    {
        nativeWindow = new wxFrame(/*parent=*/0, wxID_ANY, settings.title,
            wxDefaultPosition, wxSize(settings.width, settings.height),
            ConvertToWxStyles(settings.styles), wxFrameNameStr);
    }

    auto window = new WxWindow(settings, nativeWindow, &platform->inputManager);
    return window;
}

//-----------------------------------//

MenuBar* WxWindowManager::createMenuBar()
{
    return new WxMenuBar(new wxMenuBar());
}

//-----------------------------------//

Menu* WxWindowManager::createMenu()
{
    return new WxMenu(new wxMenu());
}

//-----------------------------------//

Window* WxWindowManager::openFileDialog(const String& wildcard,
                                        FileDialogFlags flags)
{
    wxFileDialog dialog(0);
    int res = dialog.ShowModal();
    return 0;
}

//-----------------------------------//

Window* WxWindowManager::openDirectoryDialog(const String& wildcard,
                                             DirectoryDialogFlags style)
{
    //wxDirSelector dialog;
    return 0;
}

//-----------------------------------//