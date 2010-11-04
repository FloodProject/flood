/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#ifdef VAPOR_IMAGE_PICOPNG
	#include "vapor/resources/PNG_Loader.h"
#endif

#ifdef VAPOR_IMAGE_STB
	#include "vapor/resources/STB_Image_Loader.h"
#endif

#ifdef VAPOR_IMAGE_DEVIL
	#include "vapor/resources/IL_Image_Loader.h"
#endif

#ifdef VAPOR_MESH_MILKSHAPE3D
	#include "vapor/resources/Milkshape3D_Loader.h"
#endif

#ifdef VAPOR_AUDIO_OGG
	#include "vapor/resources/OGG_Loader.h"
#endif

#ifdef VAPOR_SHADER_GLSL
	#include "vapor/resources/GLSL_Loader.h"
#endif

#ifdef VAPOR_SCRIPTING_LUA
	#include "vapor/resources/Lua_Loader.h"
#endif

#ifdef VAPOR_AUDIO_OPENAL
	#include "vapor/audio/Device.h"
#endif

#if defined(VAPOR_FONT_BITMAP) || defined(VAPOR_FONT_FREETYPE2)
	#include "vapor/resources/Font_Loader.h"
#endif