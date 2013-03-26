/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "Font.h"
#include "Image.h"
#include "Resource.h"
#include "ResourceHandle.h"
#include "Vector.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace clix;

Flood::Glyph::Glyph(::Glyph* native)
{
    BaseLineOffset = native->baseLineOffset;
    Advance = native->advance;
    Image = Flood::ResourceHandle<Flood::Image^>(native->image.id);
}

Flood::Glyph::Glyph(System::IntPtr native)
{
    auto __native = (::Glyph*)native.ToPointer();
    BaseLineOffset = __native->baseLineOffset;
    Advance = __native->advance;
    Image = Flood::ResourceHandle<Flood::Image^>(__native->image.id);
}

Flood::Font::Font(::Font* native)
    : Flood::Resource(native)
{
}

Flood::Font::Font(System::IntPtr native)
    : Flood::Resource(native)
{
    auto __native = (::Font*)native.ToPointer();
}

bool Flood::Font::GetGlyph(int codepoint, [System::Runtime::InteropServices::Out] Flood::Glyph% glyph)
{
    ::Glyph arg1;
    auto ret = ((::Font*)NativePtr)->getGlyph(codepoint, arg1);
    glyph = Flood::Glyph((::Glyph*)&arg1);
    return ret;
}

Flood::Vector2i Flood::Font::GetKerning(int codepoint1, int codepoint2)
{
    auto ret = ((::Font*)NativePtr)->getKerning(codepoint1, codepoint2);
    return Flood::Vector2i((::Vector2i*)&ret);
}

System::String^ Flood::Font::GetName()
{
    auto &ret = ((::Font*)NativePtr)->getName();
    return clix::marshalString<clix::E_UTF8>(ret);
}

void Flood::Font::SetName(System::String^ v)
{
    auto arg0 = clix::marshalString<clix::E_UTF8>(v);
    ((::Font*)NativePtr)->setName(arg0);
}

int Flood::Font::GetSize()
{
    auto ret = ((::Font*)NativePtr)->getSize();
    return ret;
}

void Flood::Font::SetSize(int v)
{
    ((::Font*)NativePtr)->setSize(v);
}

Flood::ResourceGroup Flood::Font::GetResourceGroup()
{
    auto ret = ((::Font*)NativePtr)->getResourceGroup();
    return (Flood::ResourceGroup)ret;
}

System::String^ Flood::Font::Name::get()
{
    return clix::marshalString<clix::E_UTF8>(((::Font*)NativePtr)->name);
}

void Flood::Font::Name::set(System::String^ value)
{
    ((::Font*)NativePtr)->name = clix::marshalString<clix::E_UTF8>(value);
}

int Flood::Font::Size::get()
{
    return ((::Font*)NativePtr)->size;
}

void Flood::Font::Size::set(int value)
{
    ((::Font*)NativePtr)->size = value;
}

