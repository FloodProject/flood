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

#include "Resources/PNG_Loader.h"
#include "Resources/STB_Image_Loader.h"
#include "Resources/OGG_Loader.h"
#include "Resources/ShaderLoader.h"
#include "Resources/LuaLoader.h"
#include "Resources/FontLoader.h"

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

void ReferenceLoaders()
{
#ifdef ENABLE_IMAGE_PICOPNG
	PNG_Pico_LoaderGetType();
#endif

#ifdef ENABLE_IMAGE_DEVIL
	IL_Image_LoaderGetType();
#endif

#ifdef ENABLE_IMAGE_STB
	STB_Image_LoaderGetType();
#endif

#ifdef ENABLE_AUDIO_OGG
	OGG_LoaderGetType();
#endif

#ifdef ENABLE_RENDERER_OPENGL_GLSL
	ShaderLoaderGetType();
#endif

#ifdef ENABLE_SCRIPTING_LUA
	Lua_LoaderGetType();
#endif

#ifdef ENABLE_FONT_BITMAP
	FontLoaderGetType();
#endif
}

//-----------------------------------//

NAMESPACE_RESOURCES_END