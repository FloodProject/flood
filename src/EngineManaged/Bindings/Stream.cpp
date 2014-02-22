/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Stream.h"

using namespace System;
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
    auto __ret = ((::Stream*)NativePtr)->open();
    return __ret;
}

bool Flood::Stream::Close()
{
    auto __ret = ((::Stream*)NativePtr)->close();
    return __ret;
}

long long Flood::Stream::Read(void* buffer, unsigned long long size)
{
    auto arg0 = (void*)buffer;
    auto arg1 = (::uint64)size;
    auto __ret = ((::Stream*)NativePtr)->read(arg0, arg1);
    return __ret;
}

long long Flood::Stream::Write(void* buffer, unsigned long long size)
{
    auto arg0 = (void*)buffer;
    auto arg1 = (::uint64)size;
    auto __ret = ((::Stream*)NativePtr)->write(arg0, arg1);
    return __ret;
}

void Flood::Stream::SetPosition(long long pos, Flood::StreamSeekMode mode)
{
    auto arg0 = (::int64)pos;
    auto arg1 = (::StreamSeekMode)mode;
    ((::Stream*)NativePtr)->setPosition(arg0, arg1);
}

unsigned long long Flood::Stream::Size()
{
    auto __ret = ((::Stream*)NativePtr)->size();
    return __ret;
}

void Flood::Stream::Resize(long long size)
{
    auto arg0 = (::int64)size;
    ((::Stream*)NativePtr)->resize(arg0);
}

long long Flood::Stream::Read(System::Collections::Generic::List<unsigned char>^ data)
{
    auto _tmpdata = std::vector<::uint8>();
    for each(unsigned char _element in data)
    {
        auto _marshalElement = (::uint8)(::uint8_t)_element;
        _tmpdata.push_back(_marshalElement);
    }
    auto arg0 = _tmpdata;
    auto __ret = ((::Stream*)NativePtr)->read(arg0);
    return __ret;
}

long long Flood::Stream::ReadBuffer(void* buffer, long long size)
{
    auto arg0 = (void*)buffer;
    auto arg1 = (::int64)size;
    auto __ret = ((::Stream*)NativePtr)->readBuffer(arg0, arg1);
    return __ret;
}

long long Flood::Stream::ReadString(System::String^ text)
{
    auto arg0 = clix::marshalString<clix::E_UTF8>(text);
    auto __ret = ((::Stream*)NativePtr)->readString(arg0);
    return __ret;
}

long long Flood::Stream::ReadLines(System::Collections::Generic::List<System::String^>^ lines)
{
    auto _tmplines = std::vector<::String>();
    for each(System::String^ _element in lines)
    {
        auto _marshalElement = clix::marshalString<clix::E_UTF8>(_element);
        _tmplines.push_back(_marshalElement);
    }
    auto arg0 = _tmplines;
    auto __ret = ((::Stream*)NativePtr)->readLines(arg0);
    return __ret;
}

long long Flood::Stream::Write(unsigned char* buf, unsigned long long size)
{
    auto arg0 = (::uint8*)buf;
    auto arg1 = (::uint64)size;
    auto __ret = ((::Stream*)NativePtr)->write(arg0, arg1);
    return __ret;
}

long long Flood::Stream::WriteString(System::String^ string)
{
    auto arg0 = clix::marshalString<clix::E_UTF8>(string);
    auto __ret = ((::Stream*)NativePtr)->writeString(arg0);
    return __ret;
}

bool Flood::Stream::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<Stream^>(object);

    if (!obj) return false;
    return __Instance == obj->__Instance;
}

int Flood::Stream::GetHashCode()
{
    return (int)NativePtr;
}

System::IntPtr Flood::Stream::__Instance::get()
{
    return System::IntPtr(NativePtr);
}

void Flood::Stream::__Instance::set(System::IntPtr object)
{
    NativePtr = (::Stream*)object.ToPointer();
}

long long Flood::Stream::Position::get()
{
    auto __ret = ((::Stream*)NativePtr)->getPosition();
    return __ret;
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
    auto __ret = ((::WebStream*)NativePtr)->open();
    return __ret;
}

bool Flood::WebStream::Close()
{
    auto __ret = ((::WebStream*)NativePtr)->close();
    return __ret;
}

long long Flood::WebStream::Read(void* buffer, unsigned long long size)
{
    auto arg0 = (void*)buffer;
    auto arg1 = (::uint64)size;
    auto __ret = ((::WebStream*)NativePtr)->read(arg0, arg1);
    return __ret;
}

long long Flood::WebStream::Write(void* buffer, unsigned long long size)
{
    auto arg0 = (void*)buffer;
    auto arg1 = (::uint64)size;
    auto __ret = ((::WebStream*)NativePtr)->write(arg0, arg1);
    return __ret;
}

unsigned long long Flood::WebStream::Size()
{
    auto __ret = ((::WebStream*)NativePtr)->size();
    return __ret;
}

bool Flood::WebStream::Perform()
{
    auto __ret = ((::WebStream*)NativePtr)->perform();
    return __ret;
}

bool Flood::WebStream::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<WebStream^>(object);

    if (!obj) return false;
    return __Instance == obj->__Instance;
}

int Flood::WebStream::GetHashCode()
{
    return (int)NativePtr;
}

void* Flood::WebStream::Handle::get()
{
    return ((::WebStream*)NativePtr)->handle;
}

void Flood::WebStream::Handle::set(void* value)
{
    ((::WebStream*)NativePtr)->handle = (::CURL*)value;
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
    auto __ret = ((::ZipStream*)NativePtr)->open();
    return __ret;
}

bool Flood::ZipStream::Close()
{
    auto __ret = ((::ZipStream*)NativePtr)->close();
    return __ret;
}

long long Flood::ZipStream::Read(void* buffer, unsigned long long size)
{
    auto arg0 = (void*)buffer;
    auto arg1 = (::uint64)size;
    auto __ret = ((::ZipStream*)NativePtr)->read(arg0, arg1);
    return __ret;
}

void Flood::ZipStream::SetPosition(long long pos, Flood::StreamSeekMode mode)
{
    auto arg0 = (::int64)pos;
    auto arg1 = (::StreamSeekMode)mode;
    ((::ZipStream*)NativePtr)->setPosition(arg0, arg1);
}

unsigned long long Flood::ZipStream::Size()
{
    auto __ret = ((::ZipStream*)NativePtr)->size();
    return __ret;
}

bool Flood::ZipStream::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<ZipStream^>(object);

    if (!obj) return false;
    return __Instance == obj->__Instance;
}

int Flood::ZipStream::GetHashCode()
{
    return (int)NativePtr;
}

long long Flood::ZipStream::Position::get()
{
    auto __ret = ((::ZipStream*)NativePtr)->getPosition();
    return __ret;
}

