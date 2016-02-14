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
#include "Stream.h"
#include "Vector.h"

using namespace System;
using namespace System::Runtime::InteropServices;

Flood::Image::Image(::Image* native)
    : Flood::Resource((::Resource*)native)
{
}

Flood::Image::Image(System::IntPtr native)
    : Flood::Resource(native)
{
    auto __native = (::Image*)native.ToPointer();
}

Flood::Image::Image()
    : Flood::Resource((::Resource*)nullptr)
{
    NativePtr = new ::Image();
}

Flood::Image::Image(unsigned int width, unsigned int height, Flood::PixelFormat format)
    : Flood::Resource((::Resource*)nullptr)
{
    auto arg0 = (::uint32)(::uint32_t)width;
    auto arg1 = (::uint32)(::uint32_t)height;
    auto arg2 = (::PixelFormat)format;
    NativePtr = new ::Image(arg0, arg1, arg2);
}

bool Flood::Image::IsCompressed()
{
    auto __ret = ((::Image*)NativePtr)->isCompressed();
    return __ret;
}

void Flood::Image::SetBuffer(unsigned char* data)
{
    auto arg0 = (::byte*)data;
    ((::Image*)NativePtr)->setBuffer(arg0);
}

void Flood::Image::SetBuffer(unsigned char* data, unsigned int stride)
{
    auto arg0 = (::byte*)data;
    auto arg1 = (::uint)(::uint32)(::uint32_t)stride;
    ((::Image*)NativePtr)->setBuffer(arg0, arg1);
}

void Flood::Image::SetBuffer(Flood::Image^ image, Flood::Vector2i offset)
{
    auto arg0 = (::Image*)image->NativePtr;
    auto _marshal1 = ::Vector2i();
    _marshal1.x = (::int32)(::int32_t)offset.X;
    _marshal1.y = (::int32)(::int32_t)offset.Y;
    auto arg1 = _marshal1;
    ((::Image*)NativePtr)->setBuffer(arg0, arg1);
}

void Flood::Image::Log()
{
    ((::Image*)NativePtr)->log();
}

void Flood::Image::Create(unsigned int width, unsigned int height, Flood::PixelFormat format)
{
    auto arg0 = (::uint32)(::uint32_t)width;
    auto arg1 = (::uint32)(::uint32_t)height;
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
    return __Instance == obj->__Instance;
}

int Flood::Image::GetHashCode()
{
    return (int)NativePtr;
}

Flood::ResourceHandle<Flood::Image^> Flood::Image::Create(Flood::Allocator^ _0, unsigned int width, unsigned int height, Flood::PixelFormat _1)
{
    auto arg0 = (::Allocator*)_0->NativePtr;
    auto arg1 = (::uint32)(::uint32_t)width;
    auto arg2 = (::uint32)(::uint32_t)height;
    auto arg3 = (::PixelFormat)_1;
    auto __ret = ::ImageCreate(arg0, arg1, arg2, arg3);
    return Flood::ResourceHandle<Flood::Image^>(__ret.id);
}

unsigned int Flood::Image::Width::get()
{
    auto __ret = ((::Image*)NativePtr)->getWidth();
    return __ret;
}

void Flood::Image::Width::set(unsigned int v)
{
    auto arg0 = (::uint32)(::uint32_t)v;
    ((::Image*)NativePtr)->setWidth(arg0);
}

unsigned int Flood::Image::Height::get()
{
    auto __ret = ((::Image*)NativePtr)->getHeight();
    return __ret;
}

void Flood::Image::Height::set(unsigned int v)
{
    auto arg0 = (::uint32)(::uint32_t)v;
    ((::Image*)NativePtr)->setHeight(arg0);
}

Flood::PixelFormat Flood::Image::PixelFormat::get()
{
    auto __ret = ((::Image*)NativePtr)->getPixelFormat();
    return (Flood::PixelFormat)__ret;
}

void Flood::Image::PixelFormat::set(Flood::PixelFormat v)
{
    auto arg0 = (::PixelFormat)v;
    ((::Image*)NativePtr)->setPixelFormat(arg0);
}

System::Collections::Generic::List<unsigned char>^ Flood::Image::Buffer::get()
{
    auto &__ret = ((::Image*)NativePtr)->getBuffer();
    auto _tmp__ret = gcnew System::Collections::Generic::List<unsigned char>();
    for(auto _element : __ret)
    {
        auto _marshalElement = _element;
        _tmp__ret->Add(_marshalElement);
    }
    return (System::Collections::Generic::List<unsigned char>^)(_tmp__ret);
}

void Flood::Image::Buffer::set(System::Collections::Generic::List<unsigned char>^ v)
{
    auto _tmpv = std::vector<::byte>();
    for each(unsigned char _element in v)
    {
        auto _marshalElement = (::byte)(::uint8)(::uint8_t)_element;
        _tmpv.push_back(_marshalElement);
    }
    auto arg0 = _tmpv;
    ((::Image*)NativePtr)->setBuffer(arg0);
}

System::Collections::Generic::List<unsigned char>^ Flood::Image::Buffer1::get()
{
    auto &__ret = ((::Image*)NativePtr)->getBuffer();
    auto _tmp__ret = gcnew System::Collections::Generic::List<unsigned char>();
    for(auto _element : __ret)
    {
        auto _marshalElement = _element;
        _tmp__ret->Add(_marshalElement);
    }
    return (System::Collections::Generic::List<unsigned char>^)(_tmp__ret);
}

unsigned int Flood::Image::PixelSize::get()
{
    auto __ret = ((::Image*)NativePtr)->getPixelSize();
    return __ret;
}

unsigned int Flood::Image::Size::get()
{
    auto __ret = ((::Image*)NativePtr)->getSize();
    return __ret;
}

Flood::ResourceGroup Flood::Image::ResourceGroup::get()
{
    auto __ret = ((::Image*)NativePtr)->getResourceGroup();
    return (Flood::ResourceGroup)__ret;
}

void Flood::Image::Color::set(Flood::Color color)
{
    auto _marshal0 = ::Color();
    _marshal0.r = (::byte)(::uint8)(::uint8_t)color.R;
    _marshal0.g = (::byte)(::uint8)(::uint8_t)color.G;
    _marshal0.b = (::byte)(::uint8)(::uint8_t)color.B;
    _marshal0.a = (::byte)(::uint8)(::uint8_t)color.A;
    auto arg0 = _marshal0;
    ((::Image*)NativePtr)->setColor(arg0);
}

unsigned int Flood::Image::Timestamp::get()
{
    auto __ret = ((::Image*)NativePtr)->getTimestamp();
    return __ret;
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

void Flood::ImageWriter::Save(Flood::Image^ image, Flood::Stream^ stream)
{
    auto arg0 = (::Image*)image->NativePtr;
    auto arg1 = (::Stream*)stream->NativePtr;
    ((::ImageWriter*)NativePtr)->save(arg0, arg1);
}

void Flood::ImageWriter::Save(Flood::Image^ image, System::String^ filePath)
{
    auto arg0 = (::Image*)image->NativePtr;
    auto _arg1 = clix::marshalString<clix::E_UTF8>(filePath);
    auto arg1 = _arg1.c_str();
    ((::ImageWriter*)NativePtr)->save(arg0, arg1);
}

bool Flood::ImageWriter::Convert(Flood::Image^ image)
{
    auto arg0 = (::Image*)image->NativePtr;
    auto __ret = ((::ImageWriter*)NativePtr)->convert(arg0);
    return __ret;
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
    return __Instance == obj->__Instance;
}

int Flood::ImageWriter::GetHashCode()
{
    return (int)NativePtr;
}

System::IntPtr Flood::ImageWriter::__Instance::get()
{
    return System::IntPtr(NativePtr);
}

void Flood::ImageWriter::__Instance::set(System::IntPtr object)
{
    NativePtr = (::ImageWriter*)object.ToPointer();
}
