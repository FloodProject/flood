/************************************************************************
*
* vaporEngine by triton © (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

/**
 * Manages all the available fonts on the engine.
 */

#include "vapor/resources/ResourceManager.h"
#include "vapor/resources/Font.h"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace vapor {
	namespace resources {

class FontManager : public ResourceManager
{

public:
	FontManager();
	~FontManager();

private:

	// FreeType library instance
	FT_Library ft;

	bool initFT();

};

} } // end namespaces

