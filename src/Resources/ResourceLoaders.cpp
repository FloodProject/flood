/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Resources/API.h"
#include "Resources/ResourceLoader.h"
#include "Resources/ResourceManager.h"

NAMESPACE_RESOURCES_BEGIN

//-----------------------------------//

REFLECT_ABSTRACT_CHILD_CLASS(ResourceLoader, Object)
	//FIELD_VECTOR(string, extensions) FIELD_READONLY(extensions)
REFLECT_CLASS_END()

//-----------------------------------//

ResourceStream::ResourceStream()
	: stream(nullptr)
	, loader(nullptr)
{

}

//-----------------------------------//

ResourceStream::~ResourceStream()
{
	StreamDestroy(stream);
}

//-----------------------------------//

ResourceLoadOptions::ResourceLoadOptions()
	: group(ResourceGroup::General)
	, stream(nullptr)
	, resource(nullptr)
	, asynchronousLoad(true)
	, sendLoadEvent(true)
	, isHighPriority(false)
	, keepStreamOpen(false)
{
}

//-----------------------------------//

void ResourceLoadOptions::addOption(int key, int value)
{
	option.key = key;
	option.value = value;
}

//-----------------------------------//

ExtensionMetadata* ResourceLoader::getMetadata()
{
	static ExtensionMetadata s_ResourceExtension =
	{
		"Resource Loader",
		"Extends the resource loading services to support more resource types."
	};

	return &s_ResourceExtension;
}

//-----------------------------------//

NAMESPACE_RESOURCES_END