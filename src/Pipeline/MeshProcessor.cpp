/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Pipeline/API.h"
#include "Pipeline/MeshProcessor.h"

NAMESPACE_PIPELINE_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(MeshProcessor, ResourceProcessor)
	FIELD_PRIMITIVE(2, bool, calculateBounds)
	FIELD_PRIMITIVE(3, bool, calculateTangents)
REFLECT_CLASS_END()

//-----------------------------------//

MeshProcessor::MeshProcessor()
	: calculateBounds(true)
	, calculateTangents(false)
{

}

//-----------------------------------//

ExtensionMetadata* MeshProcessor::getMetadata()
{
	static ExtensionMetadata s_MeshExtension =
	{
		"Mesh",
		"Processes a mesh resource."
	};

	return &s_MeshExtension;
}

//-----------------------------------//

bool MeshProcessor::Process(const ResourcePtr& resource)
{
	return true;
}

//-----------------------------------//

NAMESPACE_PIPELINE_END