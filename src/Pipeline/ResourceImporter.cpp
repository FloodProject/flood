/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Pipeline/API.h"
#include "Pipeline/ResourceImporter.h"
#include "Core/Log.h"

NAMESPACE_PIPELINE_BEGIN

//-----------------------------------//

REFLECT_ABSTRACT_CHILD_CLASS(ResourceImporter, ResourceLoader)
REFLECT_CLASS_END()

//-----------------------------------//

ResourceImporter::ResourceImporter()
{
}

//-----------------------------------//

ResourceImporter::~ResourceImporter()
{
}

//-----------------------------------//

NAMESPACE_PIPELINE_END