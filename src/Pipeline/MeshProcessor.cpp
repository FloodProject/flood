/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Pipeline/API.h"
#include "Pipeline/MeshProcessor.h"

NAMESPACE_PIPELINE_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(MeshProcessor, ResourceProcessor)
REFLECT_CLASS_END()

//-----------------------------------//

MeshProcessor::MeshProcessor()
{ }

//-----------------------------------//

bool MeshProcessor::Process(const ResourcePtr& resource)
{
	return true;
}

//-----------------------------------//

NAMESPACE_PIPELINE_END