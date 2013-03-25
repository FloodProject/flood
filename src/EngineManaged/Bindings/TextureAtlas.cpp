/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "TextureAtlas.h"
#include "Image.h"
#include "Rect.h"
#include "ResourceHandle.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace clix;

Flood::CompareHandle::CompareHandle(::CompareHandle* native)
{
    NativePtr = native;
}

Flood::CompareHandle::CompareHandle(System::IntPtr native)
{
    auto __native = (::CompareHandle*)native.ToPointer();
    NativePtr = __native;
}

bool Flood::CompareHandle::operator()(Flood::ResourceHandle<Flood::Image^> h1, Flood::ResourceHandle<Flood::Image^> h2)
{
    auto arg0 = (HandleId)h1.Id;
    auto arg1 = (HandleId)h2.Id;
    auto ret = ((::CompareHandle*)NativePtr)->operator()(arg0, arg1);
    return ret;
}

Flood::SubTexture::SubTexture(::SubTexture* native)
{
    NativePtr = native;
}

Flood::SubTexture::SubTexture(System::IntPtr native)
{
    auto __native = (::SubTexture*)native.ToPointer();
    NativePtr = __native;
}

Flood::Rect^ Flood::SubTexture::Rect::get()
{
    return gcnew Flood::Rect((::Rect*)&((::SubTexture*)NativePtr)->rect);
}

void Flood::SubTexture::Rect::set(Flood::Rect^ value)
{
    ((::SubTexture*)NativePtr)->rect = *(::Rect*)value->NativePtr;
}

bool Flood::SubTexture::IsRotated::get()
{
    return ((::SubTexture*)NativePtr)->isRotated;
}

void Flood::SubTexture::IsRotated::set(bool value)
{
    ((::SubTexture*)NativePtr)->isRotated = value;
}

Flood::TextureAtlas::TextureAtlas(::TextureAtlas* native)
{
    NativePtr = native;
}

Flood::TextureAtlas::TextureAtlas(System::IntPtr native)
{
    auto __native = (::TextureAtlas*)native.ToPointer();
    NativePtr = __native;
}

Flood::TextureAtlas::TextureAtlas(unsigned int maxSize)
{
    auto arg0 = (uint)(uint32)maxSize;
    NativePtr = new ::TextureAtlas(arg0);
}

bool Flood::TextureAtlas::AddImage(Flood::ResourceHandle<Flood::Image^> imageHandle)
{
    auto arg0 = (HandleId)imageHandle.Id;
    auto ret = ((::TextureAtlas*)NativePtr)->addImage(arg0);
    return ret;
}

bool Flood::TextureAtlas::GetImageSubTexture(Flood::ResourceHandle<Flood::Image^> imageHandle, Flood::SubTexture^ subTexture)
{
    auto arg0 = (HandleId)imageHandle.Id;
    auto &arg1 = *(::SubTexture*)subTexture->NativePtr;
    auto ret = ((::TextureAtlas*)NativePtr)->getImageSubTexture(arg0, arg1);
    return ret;
}

Flood::ResourceHandle<Flood::Image^> Flood::TextureAtlas::GetAtlasImageHandle()
{
    auto ret = ((::TextureAtlas*)NativePtr)->getAtlasImageHandle();
    return Flood::ResourceHandle<Flood::Image^>(ret.id);
}

