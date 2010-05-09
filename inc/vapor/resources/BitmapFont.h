/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/resources/Font.h"

#ifdef VAPOR_FONT_BITMAP

namespace vapor { namespace resources {

//-----------------------------------//

/**
 * Represents a bitmap font resource. A bitmap font is a pre-rendered
 * texture, with all the glyphs packed (usually in a grid) and also
 * normally a binary file with all the glyphs coords (UVs).
 */

class BitmapFont : public Font
{
public:

	// Gets the size information of a glyph.
	virtual const std::pair<const ushort, const ushort> getGlyphSize() const;

	// Sets the font image.
	IMPLEMENT_ACESSOR(Image, const ImagePtr&, fontImage)

	// Gets/sets the font glyphs.
	IMPLEMENT_ACESSOR(Glyphs, const std::vector<Glyph>&, glyphs)

private:

	ImagePtr fontImage;
	std::vector<Glyph> glyphs;
};

//-----------------------------------//

} } // end namespaces

#endif

