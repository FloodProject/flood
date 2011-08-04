/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#ifdef BUILDING_STANDALONE

#define INSTANTIATE_TEMPLATES

#include "Pipeline/API.h"
#include "ResourceProcessor.h"
#include "Resources/ResourceLoader.h"
#include "Resources/ResourceManager.h"

ResourceManager* resources;

//-----------------------------------//

void showResourceLoaders()
{
	printf("\nKnown resource loaders:\n\n");

	Class& type = ResourceLoader::getStaticType();
	
	for( uint i = 0; i < type.childs.size(); i++ )
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