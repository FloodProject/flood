/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "API.h"
#include "Engine/PlatformManager.h"
#include "Engine/Window/WindowManager.h"
#include "Engine/Input/InputManager.h"
#include "Engine/Input/Keyboard.h"
#include "Engine/Input/Mouse.h"

FLD_NAMESPACE_BEGIN

//-----------------------------------//

class WxApp : public wxApp { };

class WxPlatform;

class API_EDITOR WxWindowManager : public WindowManager
{
public:

    WxWindowManager(WxPlatform* platform);
    Window* createWindow(const WindowSettings& settings) override;

    Window* openFileDialog(
        const String& wildcard, FileDialogFlags style) override;

    Window* openDirectoryDialog(
        const String& wildcard, DirectoryDialogFlags style) override;

    WxPlatform* platform;
};

//-----------------------------------//

class API_EDITOR WxInputManager : public InputManager
{
public:

    virtual Keyboard* getKeyboard() override;
    virtual Mouse* getMouse() override;

    void processKeyEvent(Keyboard*, const wxKeyEvent&, bool isKeyDown);
    void processCharEvent(Keyboard*, const wxKeyEvent&);
    void processMouseEvent(Mouse*, const wxMouseEvent&);

    Keyboard keyboard;
    Mouse mouse;
};

//-----------------------------------//

/**
 * Implements a platform based on the wxWidgets GUI library.
 */
class API_EDITOR WxPlatform : public PlatformManager
{
public:

    WxPlatform();
    virtual ~WxPlatform() override;

    virtual void init() override;
    virtual void update() override;

    virtual WindowManager* getWindowManager() override;
    virtual InputManager* getInputManager() override;

    // wxWidgets main events.
    void OnIdle(wxIdleEvent& event);
    void OnClose(wxCloseEvent& event);
    void OnSize(wxSizeEvent& event);
    void OnSizing(wxSizeEvent& event);

    WxWindowManager windowManager;
    WxInputManager inputManager;
};

//-----------------------------------//

FLD_NAMESPACE_END

/**
 * This is exported as a C interface so that the managed editor code can
 * easily create an instance of of the platform without having to create
 * an heavyweight bindings project.
 */
extern "C" API_EDITOR WxPlatform* CreateWxPlatformManager();
