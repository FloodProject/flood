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
    _marshal0.x = (::int32)(::int32_t)size.X;
    _marshal0.y = (::int32)(::int32_t)size.Y;
    auto arg0 = _marshal0;
    auto arg1 = (::PixelFormat)_0;
    ((::Texture*)NativePtr)->allocate(arg0, arg1);
}

Flood::Image^ Flood::Texture::ReadImage()
{
    auto __ret = ((::Texture*)NativePtr)->readImage();
    if (__ret == nullptr) return nullptr;
    return gcnew Flood::Image((::Image*)__ret);
}

bool Flood::Texture::IsUploaded()
{
    auto __ret = ((::Texture*)NativePtr)->isUploaded();
    return __ret;
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
    auto __ret = ((::Texture*)NativePtr)->getId();
    return __ret;
}

Flood::PixelFormat Flood::Texture::PixelFormat::get()
{
    auto __ret = ((::Texture*)NativePtr)->getPixelFormat();
    return (Flood::PixelFormat)__ret;
}

Flood::ResourceHandle<Flood::Image^> Flood::Texture::Image::get()
{
    auto &__ret = ((::Texture*)NativePtr)->getImage();
    return Flood::ResourceHandle<Flood::Image^>(__ret.id);
}

void Flood::Texture::Image::set(Flood::ResourceHandle<Flood::Image^> value)
{
    auto image = value;
    auto arg0 = (HandleId)image.Id;
    ((::Texture*)NativePtr)->setImage(arg0);
}

unsigned int Flood::Texture::ExpectedSize::get()
{
    auto __ret = ((::Texture*)NativePtr)->getExpectedSize();
    return __ret;
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
    ((::Texture*)NativePtr)->width = (::uint32)(::uint32_t)value;
}

unsigned int Flood::Texture::Height::get()
{
    return ((::Texture*)NativePtr)->height;
}

void Flood::Texture::Height::set(unsigned int value)
{
    ((::Texture*)NativePtr)->height = (::uint32)(::uint32_t)value;
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
    auto __ret = ((::TextureUnit*)NativePtr)->getFilterMode();
    return (Flood::TextureFilterMode)__ret;
}

void Flood::TextureUnit::FilterMode::set(Flood::TextureFilterMode value)
{
    auto _0 = value;
    auto arg0 = (::TextureFilterMode)_0;
    ((::TextureUnit*)NativePtr)->setFilterMode(arg0);
}

Flood::TextureMipMode Flood::TextureUnit::MipMode::get()
{
    auto __ret = ((::TextureUnit*)NativePtr)->getMipMode();
    return (Flood::TextureMipMode)__ret;
}

void Flood::TextureUnit::MipMode::set(Flood::TextureMipMode value)
{
    auto _0 = value;
    auto arg0 = (::TextureMipMode)_0;
    ((::TextureUnit*)NativePtr)->setMipMode(arg0);
}

Flood::TextureWrapMode Flood::TextureUnit::WrapMode::get()
{
    auto __ret = ((::TextureUnit*)NativePtr)->getWrapMode();
    return (Flood::TextureWrapMode)__ret;
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
    ((::TextureUnit*)NativePtr)->unit = (::uint8)(::uint8_t)value;
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

