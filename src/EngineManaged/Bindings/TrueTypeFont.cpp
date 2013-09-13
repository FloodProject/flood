/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "TrueTypeFont.h"
#include "Font.h"
#include "Vector.h"
#include "Core/Array.h"

//using namespace System;
using namespace System::Runtime::InteropServices;

Flood::TrueTypeFont::TrueTypeFont(::TrueTypeFont* native)
    : Flood::Font((::Font*)native)
{
}

Flood::TrueTypeFont::TrueTypeFont(System::IntPtr native)
    : Flood::Font(native)
{
    auto __native = (::TrueTypeFont*)native.ToPointer();
}

Flood::TrueTypeFont::TrueTypeFont()
    : Flood::Font((::Font*)nullptr)
{
    NativePtr = new ::TrueTypeFont();
}

void Flood::TrueTypeFont::Init()
{
    ((::TrueTypeFont*)NativePtr)->init();
}

bool Flood::TrueTypeFont::GetGlyphInfo(int codepoint, int size, [System::Runtime::InteropServices::Out] Flood::Glyph% glyph)
{
    ::Glyph arg2;
    auto ret = ((::TrueTypeFont*)NativePtr)->getGlyphInfo(codepoint, size, arg2);
    glyph = Flood::Glyph((::Glyph*)&arg2);
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

bool Flood::TrueTypeFont::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<TrueTypeFont^>(object);

    if (!obj) return false;
    return Instance == obj->Instance;
}

int Flood::TrueTypeFont::GetHashCode()
{
    return (int)NativePtr;
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
    auto _tmpvalue = Array<::byte>(*AllocatorGetHeap());
    for each(unsigned char _element in value)
    {
        auto _marshalElement = (::byte)(::uint8)_element;
        _tmpvalue.pushBack(_marshalElement);
    }
    ((::TrueTypeFont*)NativePtr)->data = _tmpvalue;
}

