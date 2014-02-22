/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Engine/API.h"
#include "Engine/Window/Window.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

class Window;
class MenuBar;
class Menu;

/**
 * Specifies flags for opening file dialogs.
 */
enum class FLD_FLAGS FileDialogFlags
{
    Open = 1 << 1,
    Save = 1 << 2,
    AllowMultipleFiles = 1 << 3,
};

/**
 * Specifies flags for opening directory dialogs.
 */
enum class FLD_FLAGS DirectoryDialogFlags
{
    MustExist = 1 << 1,
};

/**
 * Window manager is responsible for managing the native windows in
 * the platform. It also provides support for platform-specific dialogs.
 */
class API_ENGINE WindowManager
{
public:

    virtual ~WindowManager();

    /// Creates a new window.
    virtual Window* createWindow(const WindowSettings& settings) = 0;

    /// Creates a new menu bar.
    virtual MenuBar* createMenuBar() = 0;

    /// Creates a new menu bar.
    virtual Menu* createMenu() = 0;

    /// Opens a file selection dialog.
    virtual Window* openFileDialog(const String& wildcard,
        FileDialogFlags flags) = 0;

    /// Opens a directory selection dialog.
    virtual Window* openDirectoryDialog(const String& wildcard,
        DirectoryDialogFlags flags) = 0;
};

//-----------------------------------//

NAMESPACE_ENGINE_END