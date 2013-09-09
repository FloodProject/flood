/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Engine/API.h"
#include "Engine/Subsystem.h"

FLD_NAMESPACE_BEGIN

//-----------------------------------//

class WindowManager;
class InputManager;

/**
 * This class is a platform abstraction layer responsible to provide
 * instances for platform-dependent engine subsytems like the window
 * and input managers. Each platform has one subclass that is provided
 * provided to the engine.
 */
class API_ENGINE NO_VTABLE PlatformManager : Subsystem
{
public:

    virtual ~PlatformManager() { }

    /// Initialization callback.
    virtual void init() = 0;

    /// Update callback.
    virtual void update() = 0;

    /// Creates the window manager subsystem.
    virtual WindowManager* getWindowManager() = 0;

    /// Creates the input manager subsystem.
    virtual InputManager* getInputManager() = 0;
};

//-----------------------------------//

FLD_NAMESPACE_END