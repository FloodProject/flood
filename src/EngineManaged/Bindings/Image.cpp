/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Image.h"
#include "Color.h"
#include "Memory.h"
#include "Resource.h"
#include "Vector.h"

using namespace System;
using namespace System::Runtime::InteropServices;

Flood::Image::Image(::Image* native)
    : Flood::Resource(native)
{
}

Flood::Image::Image(System::IntPtr native)
    : Flood::Resource(native)
{
    auto __native = (::Image*)native.ToPointer();
}

Flood::Image::Image()
    : Flood::Resource(nullptr)
{
    NativePtr = new ::Image();
}

Flood::Image::Image(unsigned int width, unsigned int height, Flood::PixelFormat format)
    : Flood::Resource(nullptr)
{
    auto arg0 = (uint32)width;
    auto arg1 = (uint32)height;
    auto arg2 = (::PixelFormat)format;
    NativePtr = new ::Image(arg0, arg1, arg2);
}

void Flood::Image::SetBuffer(System::Collections::Generic::List<unsigned char>^ v)
{
    auto _tmpv = std::vector<::byte>();
    for each(unsigned char _element in v)
    {
        auto _marshalElement = (byte)(uint8)_element;
        _tmpv.push_back(_marshalElement);
    }
    auto arg0 = _tmpv;
    ((::Image*)NativePtr)->setBuffer(arg0);
}

bool Flood::Image::IsCompressed()
{
    auto ret = ((::Image*)NativePtr)->isCompressed();
    return ret;
}

void Flood::Image::SetBuffer(System::IntPtr data)
{
    auto arg0 = (uint8*)data.ToPointer();
    ((::Image*)NativePtr)->setBuffer(arg0);
}

void Flood::Image::SetBuffer(System::IntPtr data, unsigned int stride)
{
    auto arg0 = (uint8*)data.ToPointer();
    auto arg1 = (uint)(uint32)stride;
    ((::Image*)NativePtr)->setBuffer(arg0, arg1);
}

void Flood::Image::SetBuffer(Flood::Image^ image, Flood::Vector2i offset)
{
    auto arg0 = (::Image*)image->NativePtr;
    auto _marshal1 = ::Vector2i();
    _marshal1.x = (int32)offset.X;
    _marshal1.y = (int32)offset.Y;
    auto arg1 = _marshal1;
    ((::Image*)NativePtr)->setBuffer(arg0, arg1);
}

void Flood::Image::Log()
{
    ((::Image*)NativePtr)->log();
}

void Flood::Image::Create(unsigned int width, unsigned int height, Flood::PixelFormat format)
{
    auto arg0 = (uint32)width;
    auto arg1 = (uint32)height;
    auto arg2 = (::PixelFormat)format;
    ((::Image*)NativePtr)->create(arg0, arg1, arg2);
}

void Flood::Image::SetModified()
{
    ((::Image*)NativePtr)->SetModified();
}

bool Flood::Image::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<Image^>(object);
    if (!obj) return false;
    return Instance == obj->Instance;
}

int Flood::Image::GetHashCode()
{
    return (int)NativePtr;
}

Flood::ResourceHandle<Flood::Image^> Flood::Image::Create(Flood::Allocator^ _0, unsigned int width, unsigned int height, Flood::PixelFormat _1)
{
    auto arg0 = (::Allocator*)_0->NativePtr;
    auto arg1 = (uint32)width;
    auto arg2 = (uint32)height;
    auto arg3 = (::PixelFormat)_1;
    auto ret = ::ImageCreate(arg0, arg1, arg2, arg3);
    return Flood::ResourceHandle<Flood::Image^>(ret.id);
}

unsigned int Flood::Image::Width::get()
{
    auto ret = ((::Image*)NativePtr)->getWidth();
    return ret;
}

void Flood::Image::Width::set(unsigned int value)
{
    auto v = value;
    auto arg0 = (uint32)v;
    ((::Image*)NativePtr)->setWidth(arg0);
}

unsigned int Flood::Image::Height::get()
{
    auto ret = ((::Image*)NativePtr)->getHeight();
    return ret;
}

void Flood::Image::Height::set(unsigned int value)
{
    auto v = value;
    auto arg0 = (uint32)v;
    ((::Image*)NativePtr)->setHeight(arg0);
}

Flood::PixelFormat Flood::Image::PixelFormat::get()
{
    auto ret = ((::Image*)NativePtr)->getPixelFormat();
    return (Flood::PixelFormat)ret;
}

void Flood::Image::PixelFormat::set(Flood::PixelFormat value)
{
    auto v = value;
    auto arg0 = (::PixelFormat)v;
    ((::Image*)NativePtr)->setPixelFormat(arg0);
}

System::Collections::Generic::List<unsigned char>^ Flood::Image::Buffer::get()
{
    auto &ret = ((::Image*)NativePtr)->getBuffer();
    auto _tmpret = gcnew System::Collections::Generic::List<unsigned char>();
    for(auto _element : ret)
    {
        auto _marshalElement = _element;
        _tmpret->Add(_marshalElement);
    }
    return _tmpret;
}

System::Collections::Generic::List<unsigned char>^ Flood::Image::Buffer1::get()
{
    auto &ret = ((::Image*)NativePtr)->getBuffer();
    auto _tmpret = gcnew System::Collections::Generic::List<unsigned char>();
    for(auto _element : ret)
    {
        auto _marshalElement = _element;
        _tmpret->Add(_marshalElement);
    }
    return _tmpret;
}

unsigned int Flood::Image::PixelSize::get()
{
    auto ret = ((::Image*)NativePtr)->getPixelSize();
    return ret;
}

unsigned int Flood::Image::Size::get()
{
    auto ret = ((::Image*)NativePtr)->getSize();
    return ret;
}

Flood::ResourceGroup Flood::Image::ResourceGroup::get()
{
    auto ret = ((::Image*)NativePtr)->getResourceGroup();
    return (Flood::ResourceGroup)ret;
}

void Flood::Image::Color::set(Flood::Color value)
{
    auto color = value;
    auto _marshal0 = ::Color();
    _marshal0.r = (byte)(uint8)color.R;
    _marshal0.g = (byte)(uint8)color.G;
    _marshal0.b = (byte)(uint8)color.B;
    _marshal0.a = (byte)(uint8)color.A;
    auto arg0 = _marshal0;
    ((::Image*)NativePtr)->setColor(arg0);
}

unsigned int Flood::Image::Timestamp::get()
{
    auto ret = ((::Image*)NativePtr)->getTimestamp();
    return ret;
}

Flood::ImageWriter::ImageWriter(::ImageWriter* native)
{
    NativePtr = native;
}

Flood::ImageWriter::ImageWriter(System::IntPtr native)
{
    auto __native = (::ImageWriter*)native.ToPointer();
    NativePtr = __native;
}

bool Flood::ImageWriter::Convert(Flood::Image^ image)
{
    auto arg0 = (::Image*)image->NativePtr;
    auto ret = ((::ImageWriter*)NativePtr)->convert(arg0);
    return ret;
}

Flood::ImageWriter::ImageWriter()
{
    NativePtr = new ::ImageWriter();
}

bool Flood::ImageWriter::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<ImageWriter^>(object);
    if (!obj) return false;
    return Instance == obj->Instance;
}

int Flood::ImageWriter::GetHashCode()
{
    return (int)NativePtr;
}

System::IntPtr Flood::ImageWriter::Instance::get()
{
    return System::IntPtr(NativePtr);
}

void Flood::ImageWriter::Instance::set(System::IntPtr object)
{
    NativePtr = (::ImageWriter*)object.ToPointer();
}
