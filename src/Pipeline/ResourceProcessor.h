/************************************************************************
*
* vapor3D © (2008-2011)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "ReferenceCount.h"
#include "Core/Class.h"
#include "Object.h"

FWD_DECL_INTRUSIVE(Resource)

namespace vapor {

//-----------------------------------//

class ResourceProcessor;
typedef std::map< const Class*, ResourceProcessor* > ResourceProcessorMap;

/**
 * Resource processors handle the pre-processing of resource data.
 */

class PIPELINE_API ResourceProcessor : public Object
{
	DECLARE_UNCOPYABLE(ResourceProcessor)
	REFLECT_DECLARE_CLASS()

public:

	// Processes the given resource.
	virtual bool Process(const ResourcePtr& resource) = 0;

	// Gets the processed resource type.
	virtual const Class& GetResourceType() = 0;

	// Registers the default resource processors.
	static void registerProcessors();

	// References the default resource processors.
	static void referenceProcessors();

	// Finds a resource processor for a given resource type.
	static ResourceProcessor* findProcessor(const Class& type);

	// Maps the resource processors to the resource types.
	static ResourceProcessorMap resourceProcessors;

protected:
	
	ResourceProcessor();
};

//-----------------------------------//

} // end namespace
