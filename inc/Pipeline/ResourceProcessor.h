/************************************************************************
*
* vapor3D © (2008-2011)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/Extension.h"

FWD_DECL_INTRUSIVE(Resource)

NAMESPACE_PIPELINE_BEGIN

//-----------------------------------//

class ResourceProcessor;
typedef std::map<Class*, ResourceProcessor*> ResourceProcessorMap;

/**
 * Resource processors handle the pre-processing of resource data.
 */

API_PIPELINE REFLECT_DECLARE_CLASS(ResourceProcessor)

class API_PIPELINE NO_VTABLE ResourceProcessor : public Extension
{
	DECLARE_UNCOPYABLE(ResourceProcessor)

public:

	virtual ~ResourceProcessor();

	// Processes the given resource.
	virtual bool Process(const ResourcePtr& resource) = 0;

	// Gets the processed resource type.
	virtual Class* GetResourceType() = 0;

protected:
	
	ResourceProcessor();
};

// Finds a resource processor for a given resource type.
API_PIPELINE ResourceProcessor* PipelineFindProcessor(Class* type);

// Maps the resource processors to the resource types.
extern std::vector<ResourceProcessor*> resourceProcessors;

//-----------------------------------//

NAMESPACE_PIPELINE_END
