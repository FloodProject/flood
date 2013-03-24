/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "Stream.h"
#include "Memory.h"
#include "ResourceHandle.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace clix;

Flood::Stream::Stream(::Stream* native)
{
    NativePtr = native;
}

Flood::Stream::Stream(System::IntPtr native)
{
    auto __native = (::Stream*)native.ToPointer();
    NativePtr = __native;
}

void Flood::Stream::Destroy()
{
    auto arg0 = (::Stream*)NativePtr;
    ::StreamDestroy(arg0);
}

bool Flood::Stream::Close()
{
    auto arg0 = (::Stream*)NativePtr;
    auto ret = ::StreamClose(arg0);
    return ret;
}

long long Flood::Stream::GetSize()
{
    auto arg0 = (::Stream*)NativePtr;
    auto ret = ::StreamGetSize(arg0);
    return ret;
}

void Flood::Stream::Resize(long long size)
{
    auto arg0 = (::Stream*)NativePtr;
    auto arg1 = (int64)size;
    ::StreamResize(arg0, arg1);
}

long long Flood::Stream::Read(System::Collections::Generic::List<unsigned char>^ data)
{
    auto arg0 = (::Stream*)NativePtr;
    auto _tmpdata = std::vector<::uint8>();
    for each(unsigned char _element in data)
    {
        auto _marshalElement = (uint8)_element;
        _tmpdata.push_back(_marshalElement);
    }
    auto arg1 = _tmpdata;
    auto ret = ::StreamRead(arg0, arg1);
    return ret;
}

long long Flood::Stream::ReadBuffer(System::IntPtr buffer, long long size)
{
    auto arg0 = (::Stream*)NativePtr;
    auto arg1 = buffer.ToPointer();
    auto arg2 = (int64)size;
    auto ret = ::StreamReadBuffer(arg0, arg1, arg2);
    return ret;
}

long long Flood::Stream::ReadString(System::String^ _1)
{
    auto arg0 = (::Stream*)NativePtr;
    auto arg1 = clix::marshalString<clix::E_UTF8>(_1);
    auto ret = ::StreamReadString(arg0, arg1);
    return ret;
}

long long Flood::Stream::ReadLines(System::Collections::Generic::List<System::String^>^ _1)
{
    auto arg0 = (::Stream*)NativePtr;
    auto _tmp_1 = std::vector<::String>();
    for each(System::String^ _element in _1)
    {
        auto _marshalElement = clix::marshalString<clix::E_UTF8>(_element);
        _tmp_1.push_back(_marshalElement);
    }
    auto arg1 = _tmp_1;
    auto ret = ::StreamReadLines(arg0, arg1);
    return ret;
}

long long Flood::Stream::Write(System::IntPtr buf, unsigned long long size)
{
    auto arg0 = (::Stream*)NativePtr;
    auto arg1 = (uint8*)buf.ToPointer();
    auto arg2 = (uint64)size;
    auto ret = ::StreamWrite(arg0, arg1, arg2);
    return ret;
}

long long Flood::Stream::WriteString(System::String^ _1)
{
    auto arg0 = (::Stream*)NativePtr;
    auto arg1 = clix::marshalString<clix::E_UTF8>(_1);
    auto ret = ::StreamWriteString(arg0, arg1);
    return ret;
}

long long Flood::Stream::GetPosition()
{
    auto arg0 = (::Stream*)NativePtr;
    auto ret = ::StreamGetPosition(arg0);
    return ret;
}

long long Flood::Stream::SetPosition(long long _1, Flood::StreamSeekMode _2)
{
    auto arg0 = (::Stream*)NativePtr;
    auto arg1 = (int64)_1;
    auto arg2 = (::StreamSeekMode)_2;
    auto ret = ::StreamSetPosition(arg0, arg1, arg2);
    return ret;
}

Flood::Stream^ Flood::Stream::CreateFromURI(Flood::Allocator^ _0, System::String^ _1, Flood::StreamOpenMode _2)
{
    auto arg0 = (::Allocator*)_0->NativePtr;
    auto arg1 = clix::marshalString<clix::E_UTF8>(_1);
    auto arg2 = (::StreamOpenMode)_2;
    auto ret = ::StreamCreateFromURI(arg0, arg1, arg2);
    return gcnew Flood::Stream((::Stream*)ret);
}

Flood::Stream^ Flood::Stream::CreateFromFile(Flood::Allocator^ _0, System::String^ _1, Flood::StreamOpenMode _2)
{
    auto arg0 = (::Allocator*)_0->NativePtr;
    auto arg1 = clix::marshalString<clix::E_UTF8>(_1);
    auto arg2 = (::StreamOpenMode)_2;
    auto ret = ::StreamCreateFromFile(arg0, arg1, arg2);
    return gcnew Flood::Stream((::Stream*)ret);
}

Flood::MemoryStream^ Flood::Stream::CreateFromMemory(Flood::Allocator^ _1, unsigned long long size)
{
    auto arg0 = (::Allocator*)_1->NativePtr;
    auto arg1 = (uint64)size;
    auto ret = ::StreamCreateFromMemory(arg0, arg1);
    return gcnew Flood::MemoryStream((::MemoryStream*)ret);
}

void Flood::Stream::MemoryInit(Flood::MemoryStream^ _0)
{
    auto arg0 = (::MemoryStream*)_0->NativePtr;
    ::StreamMemoryInit(arg0);
}

void Flood::Stream::MemorySetRawBuffer(Flood::MemoryStream^ _0, System::IntPtr buffer)
{
    auto arg0 = (::MemoryStream*)_0->NativePtr;
    auto arg1 = (uint8*)buffer.ToPointer();
    ::StreamMemorySetRawBuffer(arg0, arg1);
}

Flood::Stream^ Flood::Stream::CreateWeb(Flood::Allocator^ alloc, System::String^ URL, Flood::StreamOpenMode mode)
{
    auto arg0 = (::Allocator*)alloc->NativePtr;
    auto arg1 = clix::marshalString<clix::E_UTF8>(URL);
    auto arg2 = (::StreamOpenMode)mode;
    auto ret = ::StreamCreateWeb(arg0, arg1, arg2);
    return gcnew Flood::Stream((::Stream*)ret);
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

Flood::MemoryStream::MemoryStream(::MemoryStream* native)
    : Flood::Stream(native)
{
}

Flood::MemoryStream::MemoryStream(System::IntPtr native)
    : Flood::Stream(native)
{
    auto __native = (::MemoryStream*)native.ToPointer();
}

System::Collections::Generic::List<unsigned char>^ Flood::MemoryStream::Data::get()
{
    auto _tmpData = gcnew System::Collections::Generic::List<unsigned char>();
    for(auto _element : ((::MemoryStream*)NativePtr)->data)
    {
        auto _marshalElement = _element;
        _tmpData->Add(_marshalElement);
    }
    return _tmpData;
}

void Flood::MemoryStream::Data::set(System::Collections::Generic::List<unsigned char>^ value)
{
    auto _tmpvalue = std::vector<::uint8>();
    for each(unsigned char _element in value)
    {
        auto _marshalElement = (uint8)_element;
        _tmpvalue.push_back(_marshalElement);
    }
    ((::MemoryStream*)NativePtr)->data = _tmpvalue;
}

System::IntPtr Flood::MemoryStream::Buffer::get()
{
    return IntPtr(((::MemoryStream*)NativePtr)->buffer);
}

void Flood::MemoryStream::Buffer::set(System::IntPtr value)
{
    ((::MemoryStream*)NativePtr)->buffer = (uint8*)value.ToPointer();
}

unsigned long long Flood::MemoryStream::Position::get()
{
    return ((::MemoryStream*)NativePtr)->position;
}

void Flood::MemoryStream::Position::set(unsigned long long value)
{
    ((::MemoryStream*)NativePtr)->position = (uint64)value;
}

bool Flood::MemoryStream::UseRawBuffer::get()
{
    return ((::MemoryStream*)NativePtr)->useRawBuffer;
}

void Flood::MemoryStream::UseRawBuffer::set(bool value)
{
    ((::MemoryStream*)NativePtr)->useRawBuffer = value;
}

