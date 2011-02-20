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

namespace vapor {

//-----------------------------------//

BEGIN_CLASS_ABSTRACT(ResourceProcessor)
END_CLASS()

//-----------------------------------//

ResourceProcessorMap ResourceProcessor::resourceProcessors;
void referenceProcessors();

//-----------------------------------//

ResourceProcessor::ResourceProcessor()
{
	referenceProcessors();
}

//-----------------------------------//

void ResourceProcessor::registerProcessors()
{
	Class& type = ResourceProcessor::getStaticType();
	
	for( uint i = 0; i < type.childs.size(); i++ )
	{
		const Class* child = type.childs[i];
		
		ResourceProcessor* processor = (ResourceProcessor*) child->createInstance();
		resourceProcessors[child] = processor;
	}
}

//-----------------------------------//

ResourceProcessor* ResourceProcessor::findProcessor(const Class& type)
{
	ResourceProcessorMap::iterator it;

	for( it = resourceProcessors.begin(); it != resourceProcessors.end(); it++ )
	{
		ResourceProcessor* processor = it->second;
		
		bool isProcessor = type.inherits( processor->GetResourceType() );
		
		if( isProcessor )
			return processor;
	}

	return nullptr;
}

//-----------------------------------//

#define REF(name) name##::getStaticType();

void ResourceProcessor::referenceProcessors()
{
	REF(MeshProcessor);
	REF(ImageProcessor);
}

//-----------------------------------//

} // end namespace