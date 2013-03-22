/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "Image.h"
#include "Color.h"
#include "Memory.h"
#include "Resource.h"
#include "ResourceHandle.h"
#include "Stream.h"
#include "Vector.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace clix;

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

unsigned int Flood::Image::GetWidth()
{
    auto ret = ((::Image*)NativePtr)->getWidth();
    return ret;
}

void Flood::Image::SetWidth(unsigned int v)
{
    auto arg0 = (uint32)v;
    ((::Image*)NativePtr)->setWidth(arg0);
}

unsigned int Flood::Image::GetHeight()
{
    auto ret = ((::Image*)NativePtr)->getHeight();
    return ret;
}

void Flood::Image::SetHeight(unsigned int v)
{
    auto arg0 = (uint32)v;
    ((::Image*)NativePtr)->setHeight(arg0);
}

Flood::PixelFormat Flood::Image::GetPixelFormat()
{
    auto ret = ((::Image*)NativePtr)->getPixelFormat();
    return (Flood::PixelFormat)ret;
}

void Flood::Image::SetPixelFormat(Flood::PixelFormat v)
{
    auto arg0 = (::PixelFormat)v;
    ((::Image*)NativePtr)->setPixelFormat(arg0);
}

bool Flood::Image::IsCompressed()
{
    auto ret = ((::Image*)NativePtr)->isCompressed();
    return ret;
}

void Flood::Image::SetBuffer(System::IntPtr data, unsigned int size)
{
    auto arg0 = (uint8*)data.ToPointer();
    auto arg1 = (uint)(uint32)size;
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

unsigned int Flood::Image::GetPixelSize()
{
    auto ret = ((::Image*)NativePtr)->getPixelSize();
    return ret;
}

unsigned int Flood::Image::GetSize()
{
    auto ret = ((::Image*)NativePtr)->getSize();
    return ret;
}

Flood::ResourceGroup Flood::Image::GetResourceGroup()
{
    auto ret = ((::Image*)NativePtr)->getResourceGroup();
    return (Flood::ResourceGroup)ret;
}

void Flood::Image::SetColor(Flood::Color color)
{
    auto _marshal0 = ::Color();

    auto arg0 = _marshal0;
    ((::Image*)NativePtr)->setColor(arg0);
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

unsigned int Flood::Image::GetTimestamp()
{
    auto ret = ((::Image*)NativePtr)->getTimestamp();
    return ret;
}

Flood::ResourceHandle<Flood::Image^> Flood::Image::Create(Flood::Allocator^ _1, unsigned int width, unsigned int height, Flood::PixelFormat _2)
{
    auto arg0 = (::Allocator*)_1->NativePtr;
    auto arg1 = (uint32)width;
    auto arg2 = (uint32)height;
    auto arg3 = (::PixelFormat)_2;
    auto ret = ::ImageCreate(arg0, arg1, arg2, arg3);
    return Flood::ResourceHandle<Flood::Image^>(ret.id);
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

bool Flood::ImageWriter::Convert(Flood::Image^ image)
{
    auto arg0 = (::Image*)image->NativePtr;
    auto ret = ((::ImageWriter*)NativePtr)->convert(arg0);
    return ret;
}

