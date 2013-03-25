/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Engine/Resources/TrueTypeFont.h>
#include "Font.h"
#include "ResourceHandle.h"

namespace Flood
{
    ref class Font;
    ref class FontInfo;
    ref class Glyph;
    ref class TrueTypeFont;
    value struct Vector2i;

    /// <summary>
    /// Represents a TrueType font resource. Loads a TTF font using the
    /// cross-platform FreeType library. Parses all the font glyphs of a size, and
    /// packs them all into a big texture (texture atlas) for rendering.
    /// </summary>
    public ref class TrueTypeFont : Flood::Font
    {
    public:
        TrueTypeFont(::TrueTypeFont* native);
        TrueTypeFont(System::IntPtr native);
        TrueTypeFont(System::String^ font);
        Flood::Vector2i GetKerning(int codepoint1, int codepoint2);
    };
}
