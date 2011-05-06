/************************************************************************
*
* vapor3D © (2008-2011)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/ReferenceCount.h"
#include "Core/Object.h"

FWD_DECL_INTRUSIVE(Resource)

namespace vapor {

//-----------------------------------//

class ResourceProcessor;
typedef std::map<Class*, ResourceProcessor*> ResourceProcessorMap;

/**
 * Resource processors handle the pre-processing of resource data.
 */

REFLECT_DECLARE_CLASS(ResourceProcessor)

class PIPELINE_API NO_VTABLE ResourceProcessor : public Object
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

namespace Pipeline
{

// Initializes the default resource processors.
void Init();

// Cleans the resource processors.
void Cleanup();

// Finds a resource processor for a given resource type.
ResourceProcessor* FindProcessor(Class* type);

}

// Maps the resource processors to the resource types.
extern std::vector<ResourceProcessor*> resourceProcessors;

//-----------------------------------//

} // end namespace
