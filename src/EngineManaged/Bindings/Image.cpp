/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "Image.h"
#include "Reflection.h"
#include "Memory.h"
#include "Serialization.h"
#include "Resource.h"
#include "Color.h"
#include "Vector.h"
#include "Stream.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace clix;

Flood::Image::Image(::Image* native)
    : Resource(native)
{
}

Flood::Image::Image(System::IntPtr native)
    : Resource(native)
{
}

Flood::Image::Image()
    : Resource(nullptr)
{
    NativePtr = new ::Image();
}

Flood::Image::Image(unsigned int width, unsigned int height, Flood::PixelFormat format)
    : Resource(nullptr)
{
    auto arg0 = (uint32)width;
    auto arg1 = (uint32)height;
    auto arg2 = (::PixelFormat)format;
    NativePtr = new ::Image(arg0, arg1, arg2);
}

Flood::Class^ Flood::Image::GetType()
{
    auto ret = NativePtr->getType();
    return gcnew Flood::Class((::Class*)ret);
}

Flood::Class^ Flood::Image::GetStaticType()
{
    auto ret = NativePtr->getStaticType();
    return gcnew Flood::Class((::Class*)ret);
}

unsigned int Flood::Image::GetWidth()
{
    auto ret = NativePtr->getWidth();
    return ret;
}

void Flood::Image::SetWidth(unsigned int v)
{
    auto arg0 = (uint32)v;
    NativePtr->setWidth(arg0);
}

unsigned int Flood::Image::GetHeight()
{
    auto ret = NativePtr->getHeight();
    return ret;
}

void Flood::Image::SetHeight(unsigned int v)
{
    auto arg0 = (uint32)v;
    NativePtr->setHeight(arg0);
}

Flood::PixelFormat Flood::Image::GetPixelFormat()
{
    auto ret = NativePtr->getPixelFormat();
    return (Flood::PixelFormat)ret;
}

void Flood::Image::SetPixelFormat(Flood::PixelFormat v)
{
    auto arg0 = (::PixelFormat)v;
    NativePtr->setPixelFormat(arg0);
}

bool Flood::Image::IsCompressed()
{
    auto ret = NativePtr->isCompressed();
    return ret;
}

Flood::ResourceGroup Flood::Image::GetResourceGroup()
{
    auto ret = NativePtr->getResourceGroup();
    return (Flood::ResourceGroup)ret;
}

void Flood::Image::SetColor(Flood::Color color)
{
    auto _arg0 = (::Color*)&color;
    auto arg0 = *_arg0;
    NativePtr->setColor(arg0);
}

void Flood::Image::Log()
{
    NativePtr->log();
}

void Flood::Image::Create(unsigned int width, unsigned int height, Flood::PixelFormat format)
{
    auto arg0 = (uint32)width;
    auto arg1 = (uint32)height;
    auto arg2 = (::PixelFormat)format;
    NativePtr->create(arg0, arg1, arg2);
}

Flood::ImageWriter::ImageWriter(::ImageWriter* native)
{
    NativePtr = native;
}

Flood::ImageWriter::ImageWriter(System::IntPtr native)
{
    NativePtr = (::ImageWriter*)native.ToPointer();
}

void Flood::ImageWriter::Save(Flood::Image^ image, Flood::Stream^ stream)
{
    auto arg0 = image->NativePtr;
    auto arg1 = stream->NativePtr;
    NativePtr->save(arg0, arg1);
}

bool Flood::ImageWriter::Convert(Flood::Image^ image)
{
    auto arg0 = image->NativePtr;
    auto ret = NativePtr->convert(arg0);
    return ret;
}

