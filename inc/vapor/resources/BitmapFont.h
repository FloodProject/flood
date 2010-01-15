/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/resources/Font.h"
#include "vapor/resources/Image.h"

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

	BitmapFont( const std::string& font, int size, 
		ImagePtr img, const std::vector<Glyph>& glyphs );

	virtual ~BitmapFont();

	// Gets the texture that backs this font.
	virtual render::TexturePtr getTexture();

	// Gets the size information of a glyph.
	virtual const std::pair<const ushort, const ushort> getGlyphSize() const;

	// Gets an array with the available glyphs information.
	virtual const std::vector<Glyph>& getGlyphsTable() const;

private:

	ImagePtr fontImage;
	std::vector<Glyph> glyphData;
};

//-----------------------------------//

} } // end namespaces

#endif

