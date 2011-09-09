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
#include "Resources/IL_Image_Loader.h"
#include "Resources/OGG_Loader.h"
#include "Resources/GLSL_Loader.h"
#include "Resources/LuaLoader.h"
#include "Resources/FontLoader.h"

NAMESPACE_RESOURCES_BEGIN

//-----------------------------------//

REFLECT_ABSTRACT_CHILD_CLASS(ResourceLoader, Object)
	//FIELD_VECTOR(string, extensions) FIELD_READONLY(extensions)
REFLECT_CLASS_END()

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

#ifdef ENABLE_SHADER_GLSL
	GLSL_LoaderGetType();
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