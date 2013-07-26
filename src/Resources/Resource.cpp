/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Resources/API.h"
#include "Resources/Resource.h"
#include "Resources/ResourceLoader.h"
#include "Core/Containers/Array.h"
#include "Core/Log.h"

#ifdef API_RESOURCE_DLL_EXPORT
FL_INSTANTIATE_TEMPLATES()
#endif

NAMESPACE_RESOURCES_BEGIN

//-----------------------------------//

REFLECT_ENUM(ResourceStatus)
	ENUM(Error)
	ENUM(Unloaded)
	ENUM(Loading)
	ENUM(Loaded)
REFLECT_ENUM_END()

REFLECT_ENUM(ResourceGroup)
	ENUM(General)
	ENUM(Images)
	ENUM(Meshes)
	ENUM(Fonts)
	ENUM(Shaders)
	ENUM(Audio)
	ENUM(Scripts)
	ENUM(Scenes)
	ENUM(Materials)
	ENUM(Particles)
REFLECT_ENUM_END()

REFLECT_ABSTRACT_CHILD_CLASS(Resource, Object)
	FIELD_PRIMITIVE(0, string, path) FIELD_READONLY(path) FIELD_NO_SERIALIZE(path)
	FIELD_ENUM(1, ResourceStatus, status) FIELD_READONLY(status) FIELD_NO_SERIALIZE(status)
REFLECT_CLASS_END()

//-----------------------------------//

Resource::Resource()
	: status( ResourceStatus::Loaded )
	, stream(nullptr)
{
}

//-----------------------------------//

Resource::~Resource()
{
	Deallocate(stream);
	//LogDebug("Destroying resource '%s'", getType()->name);
}

//-----------------------------------//

bool Resource::isLoaded() const
{
	return status == ResourceStatus::Loaded;
}

//-----------------------------------//

ResourceLoader::ResourceLoader()
	: extensions(*AllocatorGetHeap())
{
}

//-----------------------------------//

ResourceLoader::~ResourceLoader()
{
	for(size_t i = 0; i < extensions.size(); ++i)
	{
		Deallocate(extensions[i]);
	}
	extensions.clear();
}

//-----------------------------------//

NAMESPACE_RESOURCES_END