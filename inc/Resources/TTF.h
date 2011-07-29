/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#ifdef ENABLE_FONT_FREETYPE2

#include "Resources/Font.h"

NAMESPACE_RESOURCES_BEGIN

//-----------------------------------//

/**
 * Represents a TTF (TrueType) font resource. Loads a TTF font using the
 * cross-platform FreeType library. Parses all the font glyphs of a size,
 * and packs them all into a big texture (texture atlas) for rendering.
 */

class TTF : public Font
{
public:

	TTF(const String& font);
	~TTF();

private:

};

//-----------------------------------//

NAMESPACE_RESOURCES_END

#endif

