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
#include "Pipeline/MilkshapeLoader.h"

namespace vapor {

//-----------------------------------//

REFLECT_ABSTRACT_CHILD_CLASS(ResourceLoader, Object)
	//FIELD_VECTOR(string, extensions) FIELD_READONLY(extensions)
REFLECT_CLASS_END()

//-----------------------------------//

#define REF(name) name##GetType();

void ResourceManager::referenceLoaders()
{
#ifdef ENABLE_IMAGE_PICOPNG
	REF(PNG_Pico_Loader)
#endif

#ifdef ENABLE_IMAGE_DEVIL
	REF(IL_Image_Loader)
#endif

#ifdef ENABLE_IMAGE_STB
	REF(STB_Image_Loader)
#endif

#ifdef ENABLE_AUDIO_OGG
	REF(OGG_Loader)
#endif

#ifdef ENABLE_SHADER_GLSL
	REF(GLSL_Loader)
#endif

#ifdef ENABLE_SCRIPTING_LUA
	REF(Lua_Loader)
#endif

#ifdef ENABLE_FONT_BITMAP
	REF(Font_Loader)
#endif

#ifdef ENABLE_MESH_MILKSHAPE3D
	REF(MilkshapeLoader)
#endif
}

//-----------------------------------//

} // end namespace