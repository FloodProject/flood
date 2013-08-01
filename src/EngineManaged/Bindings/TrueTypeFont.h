/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "CppSharp.h"
#include <Engine/Resources/TrueTypeFont.h>
#include "Font.h"
#include "Image.h"
#include "ResourceHandle.h"

namespace Flood
{
    ref class FontInfo;
    ref class TrueTypeFont;
    value struct Glyph;
    value struct Vector2;

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
        bool GetGlyphInfo(int codepoint, int size, Flood::Glyph glyph);
        Flood::ResourceHandle<Flood::Image^> CreateGlyphImage(int codepoint, int size);
        Flood::Vector2 GetKerning(int codepoint1, int codepoint2, int fontSize);
        virtual bool Equals(System::Object^ object) override;
        virtual int GetHashCode() override;
    };
}
