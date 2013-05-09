/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Core/API.h"
#include "Core/FileWatcher.h"

NAMESPACE_CORE_BEGIN

//-----------------------------------//

class FileWatcherNull : public FileWatcher
{
public:

    FileWatchId addWatch(const String& directory, void* userdata) OVERRIDE
    {
        return 0;
    }

    void removeWatch(const String& directory) OVERRIDE
    {
    }

    void removeWatch(FileWatchId FileWatchId) OVERRIDE
    {
    }

    void update() OVERRIDE
    {
    }
};

//-----------------------------------//

NAMESPACE_CORE_END