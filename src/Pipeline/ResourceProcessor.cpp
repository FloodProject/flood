/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Pipeline/API.h"
#include "Pipeline/ResourceProcessor.h"
#include "Core/Containers/Array.h"
#include "Core/Log.h"

NAMESPACE_PIPELINE_BEGIN

//-----------------------------------//

REFLECT_ABSTRACT_CHILD_CLASS(ResourceProcessor, Extension)
REFLECT_CLASS_END()

Array<ResourceProcessor*> resourceProcessors(*AllocatorGetHeap());

//-----------------------------------//

ResourceProcessor::ResourceProcessor()
{
}

//-----------------------------------//

ResourceProcessor::~ResourceProcessor()
{
}

//-----------------------------------//

NAMESPACE_PIPELINE_END