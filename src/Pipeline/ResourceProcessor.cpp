/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Pipeline/API.h"
#include "Pipeline/ResourceProcessor.h"
#include "Core/Log.h"

NAMESPACE_PIPELINE_BEGIN

//-----------------------------------//

REFLECT_ABSTRACT_CHILD_CLASS(ResourceProcessor, Object)
REFLECT_CLASS_END()

std::vector<ResourceProcessor*> resourceProcessors;

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