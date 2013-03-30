/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "TrueTypeFont.h"
#include "Font.h"
#include "Image.h"
#include "ResourceHandle.h"
#include "Vector.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace clix;

Flood::TrueTypeFont::TrueTypeFont(::TrueTypeFont* native)
    : Flood::Font(native)
{
}

Flood::TrueTypeFont::TrueTypeFont(System::IntPtr native)
    : Flood::Font(native)
{
    auto __native = (::TrueTypeFont*)native.ToPointer();
}

Flood::TrueTypeFont::TrueTypeFont()
    : Flood::Font(nullptr)
{
    NativePtr = new ::TrueTypeFont();
}

void Flood::TrueTypeFont::Init()
{
    ((::TrueTypeFont*)NativePtr)->init();
}

bool Flood::TrueTypeFont::GetGlyphInfo(int codepoint, int size, Flood::Glyph glyph)
{
    auto _marshal2 = ::Glyph();
    _marshal2.baseLineOffset = glyph.BaseLineOffset;
    _marshal2.advance = glyph.Advance;
    _marshal2.width = glyph.Width;
    _marshal2.height = glyph.Height;
    auto arg2 = _marshal2;
    auto ret = ((::TrueTypeFont*)NativePtr)->getGlyphInfo(codepoint, size, arg2);
    return ret;
}

Flood::ResourceHandle<Flood::Image^> Flood::TrueTypeFont::CreateGlyphImage(int codepoint, int size)
{
    auto ret = ((::TrueTypeFont*)NativePtr)->createGlyphImage(codepoint, size);
    return Flood::ResourceHandle<Flood::Image^>(ret.id);
}

Flood::Vector2 Flood::TrueTypeFont::GetKerning(int codepoint1, int codepoint2, int fontSize)
{
    auto ret = ((::TrueTypeFont*)NativePtr)->getKerning(codepoint1, codepoint2, fontSize);
    return Flood::Vector2((::Vector2*)&ret);
}

System::Collections::Generic::List<unsigned char>^ Flood::TrueTypeFont::Data::get()
{
    auto _tmpData = gcnew System::Collections::Generic::List<unsigned char>();
    for(auto _element : ((::TrueTypeFont*)NativePtr)->data)
    {
        auto _marshalElement = _element;
        _tmpData->Add(_marshalElement);
    }
    return _tmpData;
}

void Flood::TrueTypeFont::Data::set(System::Collections::Generic::List<unsigned char>^ value)
{
    auto _tmpvalue = std::vector<::byte>();
    for each(unsigned char _element in value)
    {
        auto _marshalElement = (byte)(uint8)_element;
        _tmpvalue.push_back(_marshalElement);
    }
    ((::TrueTypeFont*)NativePtr)->data = _tmpvalue;
}

