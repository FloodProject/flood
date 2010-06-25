/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#ifdef VAPOR_FONT_FREETYPE2

#include "vapor/resources/Font.h"

namespace vapor {

//-----------------------------------//

/**
 * Represents a TTF (TrueType) font resource. Loads a TTF font using the
 * cross-platform FreeType library. Parses all the font glyphs of a size,
 * and packs them all into a big texture (texture atlas) for rendering.
 */

class TTF : public Font
{
public:

	TTF(const std::string& font);
	~TTF();

private:

};

//-----------------------------------//

} // end namespace

#endif

