/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "Engine/API.h"

#ifdef ENABLE_FONT_FREETYPE2

#include "Resources/FontManager.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

FontManager::FontManager()
{
	if(initFT()) 
	{
		LogInfo("Initialized FreeType %d.%d.%d", 
			FREETYPE_MAJOR, FREETYPE_MINOR, FREETYPE_PATCH);
	} else
	{
		LogError("Failed to initialize FreeType");
	}
}

//-----------------------------------//

FontManager::~FontManager()
{
	if(FT_Done_FreeType(ft) != 0) 
	{
		Log::warn("Failed to destroy FreeType");
	}
}

//-----------------------------------//

bool FontManager::initFT() 
{
	// initialize FreeType library
	if(FT_Init_FreeType(&ft) == 0)
		return true;
	else
		return false;
}

//-----------------------------------//

NAMESPACE_ENGINE_END

#endif
