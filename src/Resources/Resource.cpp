/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Resources/API.h"
#include "Resources/Resource.h"

NAMESPACE_BEGIN

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
REFLECT_ENUM_END()

REFLECT_ABSTRACT_CHILD_CLASS(Resource, Object)
	FIELD_PRIMITIVE(string, path) FIELD_READONLY(path)
	FIELD_ENUM(ResourceStatus, status) FIELD_READONLY(status) FIELD_NO_SERIALIZE(status)
REFLECT_CLASS_END()

//-----------------------------------//

Resource::Resource()
	: status( ResourceStatus::Loaded )
{ }

//-----------------------------------//

bool Resource::isLoaded() const
{
	return status == ResourceStatus::Loaded;
}

//-----------------------------------//

NAMESPACE_END