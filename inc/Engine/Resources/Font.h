/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Engine/API.h"
#include "Resources/Resource.h"
#include "Graphics/Resources/Image.h"
#include "Core/Math/Vector.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

/**
 * A glyph is the information about a character/symbol in a font file.
 * This structure stores more specifically information related to its
 * position and size in the texture font file.
 */

struct Glyph
{
    float xOffset;
    float baseLineOffset;
    float advance;
    float width;
    float height;
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

REFLECT_DECLARE_CLASS(Font)

class API_ENGINE Font : public Resource
{
	REFLECT_DECLARE_OBJECT(Font)

public:

	virtual bool getGlyphInfo(int codepoint, int size, FLD_OUT Glyph& glyph) const = 0;
	virtual ImageHandle createGlyphImage(int codepoint, int size) const = 0;

	virtual Vector2 getKerning(int codepoint1, int codepoint2, int fontSize)  const = 0;

    virtual int getMaxHeight(int fontSize) const = 0;

	// Gets/sets the font name.
	ACCESSOR(Name, const String&, name)

	// Gets the resource group of the font.
	GETTER(ResourceGroup, ResourceGroup, ResourceGroup::Fonts)

	// Font name.
	String name;

protected:

	Font();
};

TYPEDEF_RESOURCE_HANDLE_FROM_TYPE( Font );

//-----------------------------------//

NAMESPACE_ENGINE_END
