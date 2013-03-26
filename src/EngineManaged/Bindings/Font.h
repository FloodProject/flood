/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Engine/Resources/Font.h>
#include "Resource.h"
#include "ResourceHandle.h"
#include "Image.h"

namespace Flood
{
    enum struct ResourceGroup;
    ref class Font;
    ref class Handle;
    ref class Image;
    ref class Resource;
    value struct Glyph;
    value struct Vector2i;

    /// <summary>
    /// A glyph is the information about a character/symbol in a font file. This
    /// structure stores more specifically information related to its position and
    /// size in the texture font file.
    /// </summary>
    public value struct Glyph
    {
    public:
        Glyph(::Glyph* native);
        Glyph(System::IntPtr native);
        int BaseLineOffset;
        float Advance;
        Flood::ResourceHandle<Flood::Image^> Image;
    };

    /// <summary>
    /// A font is a sequence of characters that can be loaded as either a
    /// pre-rendered texture with the glyph information being provided as a
    /// separate file, or as a TTF file that will be rendered to a texture at
    /// runtime and the glyph information extracted from the tables provided by the
    /// font file. With a TTF file you are not limited to a unique font size or a
    /// unique set of glyphs that were pre-rendered, so you might be able to
    /// achieve better quality using less resources, as the engine can create a
    /// unique texture with glyphs from different fonts, and with different sizes.
    /// </summary>
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
        property int Size
        {
            int get();
            void set(int);
        }
        bool GetGlyph(int codepoint, [System::Runtime::InteropServices::Out] Flood::Glyph% glyph);
        Flood::Vector2i GetKerning(int codepoint1, int codepoint2);
        System::String^ GetName();
        void SetName(System::String^ v);
        int GetSize();
        void SetSize(int v);
        Flood::ResourceGroup GetResourceGroup();
    };
}
