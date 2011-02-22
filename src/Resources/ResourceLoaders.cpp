/************************************************************************
*
* vapor3D Engine � (2008-2010)
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
#include "Resources/Lua_Loader.h"
#include "Resources/Font_Loader.h"
#include "Pipeline/MilkshapeLoader.h"

namespace vapor {

//-----------------------------------//

BEGIN_CLASS_ABSTRACT(ResourceLoader)
	//FIELD_VECTOR(string, extensions) FIELD_READONLY(extensions)
END_CLASS()

//-----------------------------------//

#define REF(name) name::getStaticType();

void ResourceManager::referenceLoaders()
{
#ifdef VAPOR_IMAGE_PICOPNG
	REF(PNG_Pico_Loader)
#endif

#ifdef VAPOR_IMAGE_DEVIL
	REF(IL_Image_Loader)
#endif

#ifdef VAPOR_IMAGE_STB
	REF(STB_Image_Loader)
#endif

#ifdef VAPOR_AUDIO_OGG
	REF(OGG_Loader)
#endif

#ifdef VAPOR_SHADER_GLSL
	REF(GLSL_Loader)
#endif

#ifdef VAPOR_SCRIPTING_LUA
	REF(Lua_Loader)
#endif

#ifdef VAPOR_FONT_BITMAP
	REF(Font_Loader)
#endif

#ifdef VAPOR_MESH_MILKSHAPE3D
	REF(MilkshapeLoader)
#endif
}

//-----------------------------------//

} // end namespace