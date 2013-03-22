/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "Texture.h"
#include "Image.h"
#include "ResourceHandle.h"
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

void Flood::Texture::Alloc(Flood::Vector2i size, Flood::PixelFormat _0)
{
    auto _marshal0 = ::Vector2i();
    _marshal0.x = (int32)size.X;
    _marshal0.y = (int32)size.Y;

    auto arg0 = _marshal0;
    auto arg1 = (::PixelFormat)_0;
    ((::Texture*)NativePtr)->allocate(arg0, arg1);
}

unsigned int Flood::Texture::GetId()
{
    auto ret = ((::Texture*)NativePtr)->getId();
    return ret;
}

Flood::PixelFormat Flood::Texture::GetPixelFormat()
{
    auto ret = ((::Texture*)NativePtr)->getPixelFormat();
    return (Flood::PixelFormat)ret;
}

Flood::Image^ Flood::Texture::GetImage()
{
    auto ret = ((::Texture*)NativePtr)->getImage();
    return gcnew Flood::Image((::Image*)ret);
}

void Flood::Texture::SetImage(Flood::Image^ image)
{
    auto arg0 = (::Image*)image->NativePtr;
    ((::Texture*)NativePtr)->setImage(arg0);
}

Flood::Image^ Flood::Texture::ReadImage()
{
    auto ret = ((::Texture*)NativePtr)->readImage();
    return gcnew Flood::Image((::Image*)ret);
}

unsigned int Flood::Texture::GetExpectedSize()
{
    auto ret = ((::Texture*)NativePtr)->getExpectedSize();
    return ret;
}

bool Flood::Texture::IsUploaded()
{
    auto ret = ((::Texture*)NativePtr)->isUploaded();
    return ret;
}

void Flood::Texture::SetUploaded()
{
    ((::Texture*)NativePtr)->setUploaded();
}

unsigned int Flood::Texture::Id::get()
{
    return ((::Texture*)NativePtr)->id;
}

void Flood::Texture::Id::set(unsigned int value)
{
    ((::Texture*)NativePtr)->id = (uint32)value;
}

Flood::TextureTarget Flood::Texture::Target::get()
{
    return (Flood::TextureTarget)((::Texture*)NativePtr)->target;
}

void Flood::Texture::Target::set(Flood::TextureTarget value)
{
    ((::Texture*)NativePtr)->target = (::TextureTarget)value;
}

unsigned short Flood::Texture::Width::get()
{
    return ((::Texture*)NativePtr)->width;
}

void Flood::Texture::Width::set(unsigned short value)
{
    ((::Texture*)NativePtr)->width = (uint16)value;
}

unsigned short Flood::Texture::Height::get()
{
    return ((::Texture*)NativePtr)->height;
}

void Flood::Texture::Height::set(unsigned short value)
{
    ((::Texture*)NativePtr)->height = (uint16)value;
}

Flood::PixelFormat Flood::Texture::Format::get()
{
    return (Flood::PixelFormat)((::Texture*)NativePtr)->format;
}

void Flood::Texture::Format::set(Flood::PixelFormat value)
{
    ((::Texture*)NativePtr)->format = (::PixelFormat)value;
}

float Flood::Texture::AnisotropicFilter::get()
{
    return ((::Texture*)NativePtr)->anisotropicFilter;
}

void Flood::Texture::AnisotropicFilter::set(float value)
{
    ((::Texture*)NativePtr)->anisotropicFilter = value;
}

Flood::Image^ Flood::Texture::Image::get()
{
    return gcnew Flood::Image((::Image*)((::Texture*)NativePtr)->image);
}

void Flood::Texture::Image::set(Flood::Image^ value)
{
    ((::Texture*)NativePtr)->image = (::Image*)value->NativePtr;
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
    auto ret = ((::TextureUnit*)NativePtr)->getFilterMode();
    return (Flood::TextureFilterMode)ret;
}

Flood::TextureMipMode Flood::TextureUnit::GetMipMode()
{
    auto ret = ((::TextureUnit*)NativePtr)->getMipMode();
    return (Flood::TextureMipMode)ret;
}

Flood::TextureWrapMode Flood::TextureUnit::GetWrapMode()
{
    auto ret = ((::TextureUnit*)NativePtr)->getWrapMode();
    return (Flood::TextureWrapMode)ret;
}

void Flood::TextureUnit::SetFilterMode(Flood::TextureFilterMode _0)
{
    auto arg0 = (::TextureFilterMode)_0;
    ((::TextureUnit*)NativePtr)->setFilterMode(arg0);
}

void Flood::TextureUnit::SetMipMode(Flood::TextureMipMode _0)
{
    auto arg0 = (::TextureMipMode)_0;
    ((::TextureUnit*)NativePtr)->setMipMode(arg0);
}

void Flood::TextureUnit::SetWrapMode(Flood::TextureWrapMode _0)
{
    auto arg0 = (::TextureWrapMode)_0;
    ((::TextureUnit*)NativePtr)->setWrapMode(arg0);
}

unsigned char Flood::TextureUnit::Unit::get()
{
    return ((::TextureUnit*)NativePtr)->unit;
}

void Flood::TextureUnit::Unit::set(unsigned char value)
{
    ((::TextureUnit*)NativePtr)->unit = (uint8)value;
}

Flood::ResourceHandle<Flood::Image^> Flood::TextureUnit::Image::get()
{
    return Flood::ResourceHandle<Flood::Image^>(((::TextureUnit*)NativePtr)->image.id);
}

void Flood::TextureUnit::Image::set(Flood::ResourceHandle<Flood::Image^> value)
{
    ((::TextureUnit*)NativePtr)->image = (HandleId)value.Id;
}

bool Flood::TextureUnit::OverrideModes::get()
{
    return ((::TextureUnit*)NativePtr)->overrideModes;
}

void Flood::TextureUnit::OverrideModes::set(bool value)
{
    ((::TextureUnit*)NativePtr)->overrideModes = value;
}

