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
    ref class TrueTypeFont;
    value struct Glyph;
    value struct Vector2;

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
        TrueTypeFont();
        property System::Collections::Generic::List<unsigned char>^ Data
        {
            System::Collections::Generic::List<unsigned char>^ get();
            void set(System::Collections::Generic::List<unsigned char>^);
        }
        void Init();
        bool CreateGlyph(int codepoint, int size, Flood::Glyph glyph);
        Flood::Vector2 GetKerning(int codepoint1, int codepoint2, int fontSize);
    };
}
