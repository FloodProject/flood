/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Core/API.h"
#include "Core/Archive.h"

NAMESPACE_CORE_BEGIN

//-----------------------------------//

Archive::Archive()
    : userdata(nullptr)
    , watchId(0)
    , isValid(true) 
{
}

//-----------------------------------//

Archive::~Archive()
{
}

//-----------------------------------//

Archive::Archive(const Path& path)
    : path(path)
    , userdata(nullptr)
    , watchId(0)
{
}

//-----------------------------------//

static Path CombinePath(const Path& path, const Path& filePath)
{
    const Path& sep = PathGetSeparator();
    Path fullPath = StringFormat("%s%s%s", 
        path.CString(), sep.CString(), filePath.CString());

    return PathNormalize(fullPath);
}

Path Archive::combinePath(const Path& filePath)
{
    return CombinePath(path, filePath);
}

//-----------------------------------//
NAMESPACE_CORE_END