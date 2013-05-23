/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Engine/API.h"

#include "Engine/Window/WindowManager.h"
#include "Core/Containers/Array.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

WindowManager::WindowManager()
    : windows(*AllocatorGetHeap())
{
}

//-----------------------------------//

WindowManager::~WindowManager()
{
}

//-----------------------------------//

void WindowManager::destroyWindows()
{
    for(auto it = array::begin(windows); it != array::end(windows); ++it)
    {
        Window* window = *it;
        Deallocate(window);
    }
}


//-----------------------------------//
NAMESPACE_ENGINE_END