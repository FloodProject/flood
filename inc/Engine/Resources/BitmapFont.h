/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Resources/Font.h"

#ifdef ENABLE_FONT_BITMAP

NAMESPACE_RESOURCES_BEGIN

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
	virtual const Vector2 getGlyphSize() const;

	// Gets/sets the font image.
	ACESSOR(Image, const ImageHandle&, image)

	// Gets/sets the font glyphs.
	ACESSOR(Glyphs, const std::vector<Glyph>&, glyphs)

private:

	ImageHandle image;
	std::vector<Glyph> glyphs;
};

//-----------------------------------//

NAMESPACE_RESOURCES_END

#endif

