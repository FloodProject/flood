/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/resources/Resource.h"
#include "vapor/resources/Image.h"
#include "vapor/math/Vector2.h"

namespace vapor {

//-----------------------------------//

/**
 * A glyph is the information about a character/symbol in a font file.
 * This structure stores more specifically information related to its
 * position and size in the texture font file.
 */

struct Glyph
{
	ushort x;
	ushort y;

	ushort width;
	ushort height;
};

//-----------------------------------//

/**
 * A font is a sequence of characters that can be loaded as either a 
 * pre-rendered texture with the glyph information being provided as
 * a separate file, or as a TTF file that will be rendered to a
 * texture at runtime and the glyph information extracted from the 
 * tables provided by the font file. With a TTF file you are not
 * limited to a unique font size or a unique set of glyphs that were
 * pre-rendered, so you might be able to achieve better quality using
 * less resources, as the engine can create a unique texture with
 * glyphs from different fonts, and with different sizes.
 */

class VAPOR_API Font : public Resource
{
public:

	// Gets the image that backs this font.
	virtual const ImagePtr& getImage() const = 0;

	// Gets an array with the available glyphs information.
	virtual const std::vector<Glyph>& getGlyphs() const = 0;

	// Gets the size information of a glyph.
	virtual const Vector2i getGlyphSize() const = 0;

	// Gets/sets the font name.
	ACESSOR(Name, const std::string&, name)

	// Gets/sets the font size.
	ACESSOR(Size, int, size)

	// Gets the resource group of the font.
	GETTER(ResourceGroup, ResourceGroup::Enum, ResourceGroup::Fonts)

protected:

	Font();

	// Font name.
	std::string name;
	
	// Font size.
	int size;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Font );

//-----------------------------------//

} // end namespace