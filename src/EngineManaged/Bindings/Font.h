/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "CppSharp.h"
#include <Engine/Resources/Font.h>
#include "Image.h"
#include "Resource.h"
#include "ResourceHandle.h"

namespace Flood
{
    enum struct ResourceGroup;
    value struct Glyph;
    value struct Vector2;

    public value struct Glyph
    {
    public:
        Glyph(::Glyph* native);
        Glyph(System::IntPtr native);
        float XOffset;
        float BaseLineOffset;
        float Advance;
        float Width;
        float Height;
    };

    public ref class Font : Flood::Resource
    {
    public:
        Font(::Font* native);
        Font(System::IntPtr native);
        property System::String^ Name
        {
            System::String^ get();
            void set(System::String^);
        }
        property Flood::ResourceGroup ResourceGroup
        {
            Flood::ResourceGroup get();
        }
        property System::String^ Name1
        {
            System::String^ get();
            void set(System::String^);
        }
        bool GetGlyphInfo(int codepoint, int size, [System::Runtime::InteropServices::Out] Flood::Glyph% glyph);
        Flood::ResourceHandle<Flood::Image^> CreateGlyphImage(int codepoint, int size);
        Flood::Vector2 GetKerning(int codepoint1, int codepoint2, int fontSize);
        virtual bool Equals(System::Object^ object) override;
        virtual int GetHashCode() override;
    };
}
