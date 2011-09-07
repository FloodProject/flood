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

#include "Pipeline/ImageProcessor.h"
#include "Pipeline/MeshProcessor.h"

#include "Core/Log.h"

NAMESPACE_PIPELINE_BEGIN

//-----------------------------------//

REFLECT_ABSTRACT_CHILD_CLASS(ResourceProcessor, Object)
REFLECT_CLASS_END()

std::vector<ResourceProcessor*> resourceProcessors;

static void ReferenceProcessors();

//-----------------------------------//

ResourceProcessor::ResourceProcessor()
{
	ReferenceProcessors();
}

//-----------------------------------//

ResourceProcessor::~ResourceProcessor()
{
}

//-----------------------------------//

static void ReferenceProcessors()
{
	MeshProcessorGetType();
	ImageProcessorGetType();
}

//-----------------------------------//

NAMESPACE_PIPELINE_END