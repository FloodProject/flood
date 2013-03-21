/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Engine/API.h"
#include "Engine/Resources/Font.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

/**
 * Represents a TrueType font resource. Loads a TTF font using the
 * cross-platform FreeType library. Parses all the font glyphs of a size,
 * and packs them all into a big texture (texture atlas) for rendering.
 */

class  API_ENGINE TrueTypeFont : public Font
{
public:

    TrueTypeFont(const String& font);
    ~TrueTypeFont();

    virtual Vector2i getKerning(int codepoint1, int codepoint2)  const OVERRIDE;

private:

    virtual bool createGlyph(int codepoint,  Glyph& glyph) const OVERRIDE;

    unsigned char ttf_buffer[1<<25];

    struct FontInfo; 
    FontInfo* fontInfo;

    float scale;
    int lineHeigth;
    int baseLine;

    mutable std::map<int, Glyph> glyphs;
};

//-----------------------------------//

NAMESPACE_ENGINE_END