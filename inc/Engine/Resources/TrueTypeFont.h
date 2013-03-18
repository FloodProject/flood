/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Engine/Resources/Font.h"
#include "stb_truetype.h"

//-----------------------------------//

/**
 * Represents a TrueType font resource. Loads a TTF font using the
 * cross-platform FreeType library. Parses all the font glyphs of a size,
 * and packs them all into a big texture (texture atlas) for rendering.
 */

class TrueTypeFont : public Font
{
public:

	TrueTypeFont(const String& font);
	~TrueTypeFont();

private:

};

//-----------------------------------//

NAMESPACE_ENGINE_END