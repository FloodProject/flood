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

BEGIN_CLASS_ABSTRACT(ResourceProcessor)
END_CLASS()

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
	Class& type = ResourceProcessor::getStaticType();
	
	for( size_t i = 0; i < type.childs.size(); i++ )
	{
		const Class* klass = type.childs[i];
		
		ResourceProcessor* processor = (ResourceProcessor*) klass->createInstance();
		resourceProcessors.push_back(processor);

		LogInfo("Registering asset handler: %s", klass->name.c_str());
	}
}

//-----------------------------------//

void Pipeline::Cleanup()
{
	for( size_t i = 0; i < resourceProcessors.size(); i++ )
	{
		ResourceProcessor* processor = resourceProcessors[i];
		delete processor;
	}

	resourceProcessors.clear();
}

//-----------------------------------//

ResourceProcessor* Pipeline::FindProcessor(const Class& type)
{
	for( size_t i = 0; i < resourceProcessors.size(); i++ )
	{
		ResourceProcessor* processor = resourceProcessors[i];
		bool isProcessor = type.inherits( processor->GetResourceType() );
		if( isProcessor ) return processor;
	}

	return nullptr;
}

//-----------------------------------//

#define REF(name) name##::getStaticType();

static void ReferenceProcessors()
{
	REF(MeshProcessor);
	REF(ImageProcessor);
}

//-----------------------------------//

} // end namespace