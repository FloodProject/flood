/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "Texture.h"
#include "Image.h"
#include "Reflection.h"
#include "Memory.h"
#include "Serialization.h"
#include "Resource.h"
#include "Color.h"
#include "Vector.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace clix;

Flood::Texture::Texture(::Texture* native)
{
    NativePtr = native;
}

Flood::Texture::Texture(System::IntPtr native)
{
    auto __native = (::Texture*)native.ToPointer();
    NativePtr = __native;
}

Flood::Texture::Texture()
{
    NativePtr = new ::Texture();
}

void Flood::Texture::Alloc(Flood::Vector2i size, Flood::PixelFormat _206)
{
    auto arg0 = ::Vector2i();
    arg0.x = (int32)size.X;
    arg0.y = (int32)size.Y;

    auto arg1 = (::PixelFormat)_206;
    NativePtr->allocate(arg0, arg1);
}

unsigned int Flood::Texture::GetId()
{
    auto ret = NativePtr->getId();
    return ret;
}

Flood::PixelFormat Flood::Texture::GetPixelFormat()
{
    auto ret = NativePtr->getPixelFormat();
    return (Flood::PixelFormat)ret;
}

Flood::Image^ Flood::Texture::GetImage()
{
    auto ret = NativePtr->getImage();
    return gcnew Flood::Image((::Image*)ret);
}

void Flood::Texture::SetImage(Flood::Image^ image)
{
    auto arg0 = image->NativePtr;
    NativePtr->setImage(arg0);
}

Flood::Image^ Flood::Texture::ReadImage()
{
    auto ret = NativePtr->readImage();
    return gcnew Flood::Image((::Image*)ret);
}

unsigned int Flood::Texture::GetExpectedSize()
{
    auto ret = NativePtr->getExpectedSize();
    return ret;
}

Flood::TextureUnit::TextureUnit(::TextureUnit* native)
{
    NativePtr = native;
}

Flood::TextureUnit::TextureUnit(System::IntPtr native)
{
    auto __native = (::TextureUnit*)native.ToPointer();
    NativePtr = __native;
}

Flood::TextureUnit::TextureUnit()
{
    NativePtr = new ::TextureUnit();
}

Flood::TextureFilterMode Flood::TextureUnit::GetFilterMode()
{
    auto ret = NativePtr->getFilterMode();
    return (Flood::TextureFilterMode)ret;
}

Flood::TextureMipMode Flood::TextureUnit::GetMipMode()
{
    auto ret = NativePtr->getMipMode();
    return (Flood::TextureMipMode)ret;
}

Flood::TextureWrapMode Flood::TextureUnit::GetWrapMode()
{
    auto ret = NativePtr->getWrapMode();
    return (Flood::TextureWrapMode)ret;
}

void Flood::TextureUnit::SetFilterMode(Flood::TextureFilterMode _208)
{
    auto arg0 = (::TextureFilterMode)_208;
    NativePtr->setFilterMode(arg0);
}

void Flood::TextureUnit::SetMipMode(Flood::TextureMipMode _209)
{
    auto arg0 = (::TextureMipMode)_209;
    NativePtr->setMipMode(arg0);
}

void Flood::TextureUnit::SetWrapMode(Flood::TextureWrapMode _210)
{
    auto arg0 = (::TextureWrapMode)_210;
    NativePtr->setWrapMode(arg0);
}

