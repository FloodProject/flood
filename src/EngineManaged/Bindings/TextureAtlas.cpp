/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "TextureAtlas.h"
#include "Image.h"
#include "Rectangle.h"

using namespace System;
using namespace System::Runtime::InteropServices;

Flood::SubTexture::SubTexture(::SubTexture* native)
{
    LeftTopUV = Flood::Vector2((::Vector2*)&native->leftTopUV);
    RightTopUV = Flood::Vector2((::Vector2*)&native->rightTopUV);
    RightBottomUV = Flood::Vector2((::Vector2*)&native->rightBottomUV);
    LeftBottomUV = Flood::Vector2((::Vector2*)&native->leftBottomUV);
    Atlas = gcnew Flood::TextureAtlas((::TextureAtlas*)native->atlas);
}

Flood::SubTexture::SubTexture(System::IntPtr native)
{
    auto __native = (::SubTexture*)native.ToPointer();
    LeftTopUV = Flood::Vector2((::Vector2*)&__native->leftTopUV);
    RightTopUV = Flood::Vector2((::Vector2*)&__native->rightTopUV);
    RightBottomUV = Flood::Vector2((::Vector2*)&__native->rightBottomUV);
    LeftBottomUV = Flood::Vector2((::Vector2*)&__native->leftBottomUV);
    Atlas = gcnew Flood::TextureAtlas((::TextureAtlas*)__native->atlas);
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

Flood::TextureAtlas::TextureAtlas(unsigned int maxSize, Flood::PixelFormat pixelFormat)
{
    auto arg0 = (::uint)(::uint32)maxSize;
    auto arg1 = (::PixelFormat)pixelFormat;
    NativePtr = new ::TextureAtlas(arg0, arg1);
}

bool Flood::TextureAtlas::AddImage(Flood::ResourceHandle<Flood::Image^> imageHandle)
{
    auto arg0 = (HandleId)imageHandle.Id;
    auto __ret = ((::TextureAtlas*)NativePtr)->addImage(arg0);
    return __ret;
}

bool Flood::TextureAtlas::GetImageSubTexture(Flood::ResourceHandle<Flood::Image^> imageHandle, [System::Runtime::InteropServices::Out] Flood::SubTexture% subTexture)
{
    auto arg0 = (HandleId)imageHandle.Id;
    ::SubTexture arg1;
    auto __ret = ((::TextureAtlas*)NativePtr)->getImageSubTexture(arg0, arg1);
    subTexture = Flood::SubTexture((::SubTexture*)&arg1);
    return __ret;
}

bool Flood::TextureAtlas::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<TextureAtlas^>(object);

    if (!obj) return false;
    return Instance == obj->Instance;
}

int Flood::TextureAtlas::GetHashCode()
{
    return (int)NativePtr;
}

System::IntPtr Flood::TextureAtlas::Instance::get()
{
    return System::IntPtr(NativePtr);
}

void Flood::TextureAtlas::Instance::set(System::IntPtr object)
{
    NativePtr = (::TextureAtlas*)object.ToPointer();
}

Flood::ResourceHandle<Flood::Image^> Flood::TextureAtlas::AtlasImageHandle::get()
{
    auto __ret = ((::TextureAtlas*)NativePtr)->getAtlasImageHandle();
    return Flood::ResourceHandle<Flood::Image^>(__ret.id);
}

