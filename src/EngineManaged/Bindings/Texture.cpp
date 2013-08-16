/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Texture.h"
#include "Image.h"
#include "Vector.h"

using namespace System;
using namespace System::Runtime::InteropServices;

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
    _marshal0.x = (::int32)size.X;
    _marshal0.y = (::int32)size.Y;
    auto arg0 = _marshal0;
    auto arg1 = (::PixelFormat)_0;
    ((::Texture*)NativePtr)->allocate(arg0, arg1);
}

Flood::Image^ Flood::Texture::ReadImage()
{
    auto ret = ((::Texture*)NativePtr)->readImage();
    return gcnew Flood::Image((::Image*)ret);
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

bool Flood::Texture::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<Texture^>(object);

    if (!obj) return false;
    return Instance == obj->Instance;
}

int Flood::Texture::GetHashCode()
{
    return (int)NativePtr;
}

System::IntPtr Flood::Texture::Instance::get()
{
    return System::IntPtr(NativePtr);
}

void Flood::Texture::Instance::set(System::IntPtr object)
{
    NativePtr = (::Texture*)object.ToPointer();
}

unsigned int Flood::Texture::Id::get()
{
    auto ret = ((::Texture*)NativePtr)->getId();
    return ret;
}

Flood::PixelFormat Flood::Texture::PixelFormat::get()
{
    auto ret = ((::Texture*)NativePtr)->getPixelFormat();
    return (Flood::PixelFormat)ret;
}

Flood::ResourceHandle<Flood::Image^> Flood::Texture::Image::get()
{
    auto &ret = ((::Texture*)NativePtr)->getImage();
    return Flood::ResourceHandle<Flood::Image^>(ret.id);
}

void Flood::Texture::Image::set(Flood::ResourceHandle<Flood::Image^> value)
{
    auto image = value;
    auto arg0 = (HandleId)image.Id;
    ((::Texture*)NativePtr)->setImage(arg0);
}

unsigned int Flood::Texture::ExpectedSize::get()
{
    auto ret = ((::Texture*)NativePtr)->getExpectedSize();
    return ret;
}

unsigned int Flood::Texture::Id1::get()
{
    return ((::Texture*)NativePtr)->id;
}

void Flood::Texture::Id1::set(unsigned int value)
{
    ((::Texture*)NativePtr)->id = (::uint32)value;
}

Flood::TextureTarget Flood::Texture::Target::get()
{
    return (Flood::TextureTarget)((::Texture*)NativePtr)->target;
}

void Flood::Texture::Target::set(Flood::TextureTarget value)
{
    ((::Texture*)NativePtr)->target = (::TextureTarget)value;
}

unsigned int Flood::Texture::Width::get()
{
    return ((::Texture*)NativePtr)->width;
}

void Flood::Texture::Width::set(unsigned int value)
{
    ((::Texture*)NativePtr)->width = (::uint32)value;
}

unsigned int Flood::Texture::Height::get()
{
    return ((::Texture*)NativePtr)->height;
}

void Flood::Texture::Height::set(unsigned int value)
{
    ((::Texture*)NativePtr)->height = (::uint32)value;
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

bool Flood::TextureUnit::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<TextureUnit^>(object);

    if (!obj) return false;
    return Instance == obj->Instance;
}

int Flood::TextureUnit::GetHashCode()
{
    return (int)NativePtr;
}

System::IntPtr Flood::TextureUnit::Instance::get()
{
    return System::IntPtr(NativePtr);
}

void Flood::TextureUnit::Instance::set(System::IntPtr object)
{
    NativePtr = (::TextureUnit*)object.ToPointer();
}

Flood::TextureFilterMode Flood::TextureUnit::FilterMode::get()
{
    auto ret = ((::TextureUnit*)NativePtr)->getFilterMode();
    return (Flood::TextureFilterMode)ret;
}

void Flood::TextureUnit::FilterMode::set(Flood::TextureFilterMode value)
{
    auto _0 = value;
    auto arg0 = (::TextureFilterMode)_0;
    ((::TextureUnit*)NativePtr)->setFilterMode(arg0);
}

Flood::TextureMipMode Flood::TextureUnit::MipMode::get()
{
    auto ret = ((::TextureUnit*)NativePtr)->getMipMode();
    return (Flood::TextureMipMode)ret;
}

void Flood::TextureUnit::MipMode::set(Flood::TextureMipMode value)
{
    auto _0 = value;
    auto arg0 = (::TextureMipMode)_0;
    ((::TextureUnit*)NativePtr)->setMipMode(arg0);
}

Flood::TextureWrapMode Flood::TextureUnit::WrapMode::get()
{
    auto ret = ((::TextureUnit*)NativePtr)->getWrapMode();
    return (Flood::TextureWrapMode)ret;
}

void Flood::TextureUnit::WrapMode::set(Flood::TextureWrapMode value)
{
    auto _0 = value;
    auto arg0 = (::TextureWrapMode)_0;
    ((::TextureUnit*)NativePtr)->setWrapMode(arg0);
}

unsigned char Flood::TextureUnit::Unit::get()
{
    return ((::TextureUnit*)NativePtr)->unit;
}

void Flood::TextureUnit::Unit::set(unsigned char value)
{
    ((::TextureUnit*)NativePtr)->unit = (::uint8)value;
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

