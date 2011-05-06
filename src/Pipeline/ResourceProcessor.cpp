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

namespace vapor {

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

void Pipeline::Init()
{
	Class* klass = ResourceProcessorGetType();
	
	for( size_t i = 0; i < klass->childs.size(); i++ )
	{
		Class* child = klass->childs[i];
		
		ResourceProcessor* processor = (ResourceProcessor*) ClassCreateInstance(child, AllocatorGetHeap());
		resourceProcessors.push_back(processor);

		LogInfo("Registering asset handler: %s", child->name);
	}
}

//-----------------------------------//

void Pipeline::Cleanup()
{
	for( size_t i = 0; i < resourceProcessors.size(); i++ )
	{
		ResourceProcessor* processor = resourceProcessors[i];
		Deallocate(processor);
	}

	resourceProcessors.clear();
}

//-----------------------------------//

ResourceProcessor* Pipeline::FindProcessor(Class* type)
{
	for( size_t i = 0; i < resourceProcessors.size(); i++ )
	{
		ResourceProcessor* processor = resourceProcessors[i];
		
		bool isProcessor = ClassInherits(type, processor->GetResourceType());
		if( isProcessor ) return processor;
	}

	return nullptr;
}

//-----------------------------------//

#define REF(name) name##GetType();

static void ReferenceProcessors()
{
	REF(MeshProcessor);
	REF(ImageProcessor);
}

//-----------------------------------//

} // end namespace