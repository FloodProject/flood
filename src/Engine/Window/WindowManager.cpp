/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Engine/API.h"
#include "Engine/Window/WindowManager.h"
#include "Core/Array.h"

//-----------------------------------//

WindowManager::WindowManager()
{
}

//-----------------------------------//

WindowManager::~WindowManager()
{
}

//-----------------------------------//

void WindowManager::destroyWindows()
{
    for(auto w : windows)
    {
        Deallocate(w);
    }
}


//-----------------------------------//
