/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Pipeline/API.h"
#include "Pipeline/ResourceProcessor.h"
#include "Resources/ResourceLoader.h"
#include "Resources/ResourceManager.h"
#include "Core/Log.h"

#include "Pipeline/ImageProcessor.h"
#include "Pipeline/MeshProcessor.h"

#include "Pipeline/ImporterMilkshape.h"
#include "Pipeline/ImporterFBX.h"

#ifdef API_PIPELINE_DLL_EXPORT
FL_INSTANTIATE_TEMPLATES()
#endif

//-----------------------------------//

static void ReferenceProcessors()
{
	MeshProcessorGetType();
	ImageProcessorGetType();
}

//-----------------------------------//

static void ReferenceImporters()
{
#ifdef ENABLE_IMPORTER_MILKSHAPE
	ImporterMilkshapeGetType();
#endif

#ifdef ENABLE_IMPORTER_FBX
	ImporterFBXGetType();
#endif
}

//-----------------------------------//

void PipelineInit()
{
	ReferenceProcessors();
	ReferenceImporters();

	Class* klass = ResourceProcessorGetType();
	
	for(auto& child : klass->childs)
	{
		auto processor = (ResourceProcessor*) 
			child->createInstance(AllocatorGetHeap());
		resourceProcessors.push_back(processor);

		LogInfo("Registering asset handler: %s", child->name);
	}
}

//-----------------------------------//

void PipelineCleanup()
{
	for(auto& processor : resourceProcessors)
	{
		Deallocate(processor);
	}

	resourceProcessors.clear();
}

//-----------------------------------//

ResourceProcessor* PipelineFindProcessor(Class* type)
{
	for(auto& processor : resourceProcessors)
		if (type->inherits(processor->GetResourceType())) return processor;

	return nullptr;
}

//-----------------------------------//

#ifdef BUILDING_STANDALONE

#define INSTANTIATE_TEMPLATES

ResourceManager* resources;

//-----------------------------------//

void showResourceLoaders()
{
	printf("\nKnown resource loaders:\n\n");

	Class& type = ResourceLoader::getStaticType();
	
	for( size_t i = 0; i < type.childs.size(); i++ )
	{
		Class& child = *type.childs[i];
		printf("\t%s\n", child.name.c_str());
	} 
}

//-----------------------------------//

void showResourceProcessors()
{
	printf("\nKnown resource processors:\n\n");

	Class& type = ResourceProcessor::getStaticType();
	
	for( size_t i = 0; i < type.childs.size(); i++ )
	{
		Class& child = *type.childs[i];
		printf("\t%s\n", child.name.c_str());
	}
}

//-----------------------------------//

int main(int argc, char* argv[])
{
	printf("Syntax: Pipeline.exe -in res.foo -out res.bar\n");

	resources = new ResourceManager();

	showResourceLoaders();
	showResourceProcessors();

	return 0;
}

//-----------------------------------//

#endif