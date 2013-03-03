/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Engine/API.h"
#include "Engine/Window/Window.h"
#include <vector>

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

class Window;

/**
 * Window manager manages windows (Windows baby!).
 */

class API_ENGINE WindowManager
{
public:

    WindowManager();
    virtual ~WindowManager();

    // Creates a new window.
    virtual Window* createWindow(const WindowSettings& settings) = 0;

    // Destroys all the windows.
    void destroyWindows();

    std::vector<Window*> windows;
};

//-----------------------------------//

NAMESPACE_ENGINE_END