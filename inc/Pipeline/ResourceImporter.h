/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Pipeline/API.h"
#include "Resources/ResourceLoader.h"

NAMESPACE_PIPELINE_BEGIN

//-----------------------------------//

/**
 * Resource importers are like resource loaders, but are intended for
 * assets that are processed and not loaded at runtime.
 */

API_PIPELINE REFLECT_DECLARE_CLASS(ResourceImporter)

class API_PIPELINE NO_VTABLE ResourceImporter : public ResourceLoader
{
    DECLARE_UNCOPYABLE(ResourceImporter)
    REFLECT_DECLARE_OBJECT(ResourceImporter)

public:

    ResourceImporter();
    virtual ~ResourceImporter();
};

//-----------------------------------//

NAMESPACE_PIPELINE_END