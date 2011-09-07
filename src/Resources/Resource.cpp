/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Resources/API.h"
#include "Resources/Resource.h"
#include "Core/Log.h"

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
	FIELD_PRIMITIVE(0, string, path) FIELD_READONLY(path)
	FIELD_ENUM(1, ResourceStatus, status) FIELD_READONLY(status) FIELD_NO_SERIALIZE(status)
REFLECT_CLASS_END()

//-----------------------------------//

Resource::Resource()
	: status( ResourceStatus::Loaded )
{
}

//-----------------------------------//

Resource::~Resource()
{
	LogDebug("Destroying resource '%s'", getType()->name);
}

//-----------------------------------//

bool Resource::isLoaded() const
{
	return status == ResourceStatus::Loaded;
}

//-----------------------------------//

NAMESPACE_RESOURCES_END