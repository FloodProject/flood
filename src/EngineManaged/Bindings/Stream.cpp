/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Stream.h"

#include "Core/Array.h"

//using namespace System;
using namespace System::Runtime::InteropServices;

Flood::Stream::Stream(::Stream* native)
{
    NativePtr = native;
}

Flood::Stream::Stream(System::IntPtr native)
{
    auto __native = (::Stream*)native.ToPointer();
    NativePtr = __native;
}

bool Flood::Stream::Open()
{
    auto ret = ((::Stream*)NativePtr)->open();
    return ret;
}

bool Flood::Stream::Close()
{
    auto ret = ((::Stream*)NativePtr)->close();
    return ret;
}

long long Flood::Stream::Read(System::IntPtr buffer, unsigned long long size)
{
    auto arg0 = buffer.ToPointer();
    auto arg1 = (::uint64)size;
    auto ret = ((::Stream*)NativePtr)->read(arg0, arg1);
    return ret;
}

long long Flood::Stream::Write(System::IntPtr buffer, unsigned long long size)
{
    auto arg0 = buffer.ToPointer();
    auto arg1 = (::uint64)size;
    auto ret = ((::Stream*)NativePtr)->write(arg0, arg1);
    return ret;
}

void Flood::Stream::SetPosition(long long pos, Flood::StreamSeekMode mode)
{
    auto arg0 = (::int64)pos;
    auto arg1 = (::StreamSeekMode)mode;
    ((::Stream*)NativePtr)->setPosition(arg0, arg1);
}

unsigned long long Flood::Stream::Size()
{
    auto ret = ((::Stream*)NativePtr)->size();
    return ret;
}

void Flood::Stream::Resize(long long size)
{
    auto arg0 = (::int64)size;
    ((::Stream*)NativePtr)->resize(arg0);
}

long long Flood::Stream::Read(System::Collections::Generic::List<unsigned char>^ data)
{
    auto _tmpdata = ::Array<::uint8>(*AllocatorGetHeap());
    for each(unsigned char _element in data)
    {
        auto _marshalElement = (::uint8)_element;
        _tmpdata.pushBack(_marshalElement);
    }
    auto arg0 = _tmpdata;
    auto ret = ((::Stream*)NativePtr)->read(arg0);
    return ret;
}

long long Flood::Stream::ReadBuffer(System::IntPtr buffer, long long size)
{
    auto arg0 = buffer.ToPointer();
    auto arg1 = (::int64)size;
    auto ret = ((::Stream*)NativePtr)->readBuffer(arg0, arg1);
    return ret;
}

long long Flood::Stream::ReadString(System::String^ text)
{
    auto arg0 = clix::marshalString<clix::E_UTF8>(text);
    auto ret = ((::Stream*)NativePtr)->readString(arg0);
    return ret;
}

long long Flood::Stream::ReadLines(System::Collections::Generic::List<System::String^>^ lines)
{
    auto _tmplines = ::Array<::String>(*AllocatorGetHeap());
    for each(System::String^ _element in lines)
    {
        auto _marshalElement = clix::marshalString<clix::E_UTF8>(_element);
        _tmplines.pushBack(_marshalElement);
    }
    auto arg0 = _tmplines;
    auto ret = ((::Stream*)NativePtr)->readLines(arg0);
    return ret;
}

long long Flood::Stream::Write1(System::IntPtr buf, unsigned long long size)
{
    auto arg0 = (uint8*)buf.ToPointer();
    auto arg1 = (::uint64)size;
    auto ret = ((::Stream*)NativePtr)->write(arg0, arg1);
    return ret;
}

long long Flood::Stream::WriteString(System::String^ string)
{
    auto arg0 = clix::marshalString<clix::E_UTF8>(string);
    auto ret = ((::Stream*)NativePtr)->writeString(arg0);
    return ret;
}

bool Flood::Stream::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<Stream^>(object);

    if (!obj) return false;
    return Instance == obj->Instance;
}

int Flood::Stream::GetHashCode()
{
    return (int)NativePtr;
}

System::IntPtr Flood::Stream::Instance::get()
{
    return System::IntPtr(NativePtr);
}

void Flood::Stream::Instance::set(System::IntPtr object)
{
    NativePtr = (::Stream*)object.ToPointer();
}

long long Flood::Stream::Position::get()
{
    auto ret = ((::Stream*)NativePtr)->getPosition();
    return ret;
}

System::String^ Flood::Stream::Path::get()
{
    return clix::marshalString<clix::E_UTF8>(((::Stream*)NativePtr)->path);
}

void Flood::Stream::Path::set(System::String^ value)
{
    ((::Stream*)NativePtr)->path = clix::marshalString<clix::E_UTF8>(value);
}

Flood::StreamOpenMode Flood::Stream::Mode::get()
{
    return (Flood::StreamOpenMode)((::Stream*)NativePtr)->mode;
}

void Flood::Stream::Mode::set(Flood::StreamOpenMode value)
{
    ((::Stream*)NativePtr)->mode = (::StreamOpenMode)value;
}

Flood::WebStream::WebStream(::WebStream* native)
    : Flood::Stream((::Stream*)native)
{
}

Flood::WebStream::WebStream(System::IntPtr native)
    : Flood::Stream(native)
{
    auto __native = (::WebStream*)native.ToPointer();
}

Flood::WebStream::WebStream(System::String^ URL, Flood::StreamOpenMode mode)
    : Flood::Stream((::Stream*)nullptr)
{
    auto arg0 = clix::marshalString<clix::E_UTF8>(URL);
    auto arg1 = (::StreamOpenMode)mode;
    NativePtr = new ::WebStream(arg0, arg1);
}

bool Flood::WebStream::Open()
{
    auto ret = ((::WebStream*)NativePtr)->open();
    return ret;
}

bool Flood::WebStream::Close()
{
    auto ret = ((::WebStream*)NativePtr)->close();
    return ret;
}

long long Flood::WebStream::Read(System::IntPtr buffer, unsigned long long size)
{
    auto arg0 = buffer.ToPointer();
    auto arg1 = (::uint64)size;
    auto ret = ((::WebStream*)NativePtr)->read(arg0, arg1);
    return ret;
}

long long Flood::WebStream::Write(System::IntPtr buffer, unsigned long long size)
{
    auto arg0 = buffer.ToPointer();
    auto arg1 = (::uint64)size;
    auto ret = ((::WebStream*)NativePtr)->write(arg0, arg1);
    return ret;
}

unsigned long long Flood::WebStream::Size()
{
    auto ret = ((::WebStream*)NativePtr)->size();
    return ret;
}

bool Flood::WebStream::Perform()
{
    auto ret = ((::WebStream*)NativePtr)->perform();
    return ret;
}

bool Flood::WebStream::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<WebStream^>(object);

    if (!obj) return false;
    return Instance == obj->Instance;
}

int Flood::WebStream::GetHashCode()
{
    return (int)NativePtr;
}

System::IntPtr Flood::WebStream::Handle::get()
{
    return System::IntPtr(((::WebStream*)NativePtr)->handle);
}

void Flood::WebStream::Handle::set(System::IntPtr value)
{
    ((::WebStream*)NativePtr)->handle = value.ToPointer();
}

bool Flood::WebStream::IsPerformDone::get()
{
    return ((::WebStream*)NativePtr)->isPerformDone;
}

void Flood::WebStream::IsPerformDone::set(bool value)
{
    ((::WebStream*)NativePtr)->isPerformDone = value;
}

Flood::ZipStream::ZipStream(::ZipStream* native)
    : Flood::Stream((::Stream*)native)
{
}

Flood::ZipStream::ZipStream(System::IntPtr native)
    : Flood::Stream(native)
{
    auto __native = (::ZipStream*)native.ToPointer();
}

bool Flood::ZipStream::Open()
{
    auto ret = ((::ZipStream*)NativePtr)->open();
    return ret;
}

bool Flood::ZipStream::Close()
{
    auto ret = ((::ZipStream*)NativePtr)->close();
    return ret;
}

long long Flood::ZipStream::Read(System::IntPtr buffer, unsigned long long size)
{
    auto arg0 = buffer.ToPointer();
    auto arg1 = (::uint64)size;
    auto ret = ((::ZipStream*)NativePtr)->read(arg0, arg1);
    return ret;
}

void Flood::ZipStream::SetPosition(long long pos, Flood::StreamSeekMode mode)
{
    auto arg0 = (::int64)pos;
    auto arg1 = (::StreamSeekMode)mode;
    ((::ZipStream*)NativePtr)->setPosition(arg0, arg1);
}

unsigned long long Flood::ZipStream::Size()
{
    auto ret = ((::ZipStream*)NativePtr)->size();
    return ret;
}

bool Flood::ZipStream::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<ZipStream^>(object);

    if (!obj) return false;
    return Instance == obj->Instance;
}

int Flood::ZipStream::GetHashCode()
{
    return (int)NativePtr;
}

long long Flood::ZipStream::Position::get()
{
    auto ret = ((::ZipStream*)NativePtr)->getPosition();
    return ret;
}

