/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/resources/FontManager.h"

#ifdef VAPOR_FONT_FREETYPE2

namespace vapor {
	namespace resources {

//-----------------------------------//

FontManager::FontManager()
{
	if(initFT()) 
	{
		info("resources::ttf", "Initialized FreeType %d.%d.%d", 
			FREETYPE_MAJOR, FREETYPE_MINOR, FREETYPE_PATCH);
	} else {
		error("resources::ttf", "Failed to initialize FreeType");
		exit(EXIT_FAILURE);
	}
}

//-----------------------------------//

FontManager::~FontManager()
{
	if(FT_Done_FreeType(ft) != 0) 
	{
		warn("resources::ttf", "Failed to destroy FreeType");
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

} } // end namespaces

#endif
