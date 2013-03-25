/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "TrueTypeFont.h"
#include "Font.h"
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

Flood::TrueTypeFont::TrueTypeFont(System::String^ font)
    : Flood::Font(nullptr)
{
    auto arg0 = clix::marshalString<clix::E_UTF8>(font);
    NativePtr = new ::TrueTypeFont(arg0);
}

Flood::Vector2i Flood::TrueTypeFont::GetKerning(int codepoint1, int codepoint2)
{
    auto ret = ((::TrueTypeFont*)NativePtr)->getKerning(codepoint1, codepoint2);
    return Flood::Vector2i((::Vector2i*)&ret);
}

