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

class  API_ENGINE FreeTypeFont : public Font
{
public:

    FreeTypeFont();
    ~FreeTypeFont();

    void init();

    virtual bool getGlyphInfo(int codepoint, int size, Glyph& glyph) const OVERRIDE;
    virtual ImageHandle createGlyphImage(int codepoint, int size) const OVERRIDE;

    virtual Vector2 getKerning(int codepoint1, int codepoint2, int fontSize)  const OVERRIDE;

    virtual int getMaxHeight(int fontSize) const OVERRIDE;

    Vector<byte> data;

private:

    struct FontInfo; 
    FontInfo* fontInfo;

    int fontSize;

};

//-----------------------------------//

NAMESPACE_ENGINE_END