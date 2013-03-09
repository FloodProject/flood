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

Flood::Stream^ Flood::Stream::CreateFromURI(Flood::Allocator^ _123, System::String^ _124, Flood::StreamOpenMode _125)
{
    auto arg0 = (::Allocator*)_123->NativePtr;
    auto arg1 = marshalString<E_UTF8>(_124);
    auto arg2 = (::StreamOpenMode)_125;
    auto ret = ::StreamCreateFromURI(arg0, arg1, arg2);
    return gcnew Flood::Stream((::Stream*)ret);
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

long long Flood::Stream::ReadBuffer(System::IntPtr buffer, long long size)
{
    auto arg0 = (::Stream*)NativePtr;
    auto arg1 = buffer.ToPointer();
    auto arg2 = (int64)size;
    auto ret = ::StreamReadBuffer(arg0, arg1, arg2);
    return ret;
}

long long Flood::Stream::ReadString(System::String^ _133)
{
    auto arg0 = (::Stream*)NativePtr;
    auto arg1 = marshalString<E_UTF8>(_133);
    auto ret = ::StreamReadString(arg0, arg1);
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

long long Flood::Stream::WriteString(System::String^ _138)
{
    auto arg0 = (::Stream*)NativePtr;
    auto arg1 = marshalString<E_UTF8>(_138);
    auto ret = ::StreamWriteString(arg0, arg1);
    return ret;
}

long long Flood::Stream::GetPosition()
{
    auto arg0 = (::Stream*)NativePtr;
    auto ret = ::StreamGetPosition(arg0);
    return ret;
}

long long Flood::Stream::SetPosition(long long _141, Flood::StreamSeekMode _142)
{
    auto arg0 = (::Stream*)NativePtr;
    auto arg1 = (int64)_141;
    auto arg2 = (::StreamSeekMode)_142;
    auto ret = ::StreamSetPosition(arg0, arg1, arg2);
    return ret;
}

Flood::Stream^ Flood::Stream::CreateFromFile(Flood::Allocator^ _143, System::String^ _144, Flood::StreamOpenMode _145)
{
    auto arg0 = (::Allocator*)_143->NativePtr;
    auto arg1 = marshalString<E_UTF8>(_144);
    auto arg2 = (::StreamOpenMode)_145;
    auto ret = ::StreamCreateFromFile(arg0, arg1, arg2);
    return gcnew Flood::Stream((::Stream*)ret);
}

Flood::MemoryStream^ Flood::Stream::CreateFromMemory(Flood::Allocator^ _146, unsigned long long size)
{
    auto arg0 = (::Allocator*)_146->NativePtr;
    auto arg1 = (uint64)size;
    auto ret = ::StreamCreateFromMemory(arg0, arg1);
    return gcnew Flood::MemoryStream((::MemoryStream*)ret);
}

void Flood::Stream::MemoryInit(Flood::MemoryStream^ _147)
{
    auto arg0 = (::MemoryStream*)_147->NativePtr;
    ::StreamMemoryInit(arg0);
}

void Flood::Stream::MemorySetRawBuffer(Flood::MemoryStream^ _148, System::IntPtr buffer)
{
    auto arg0 = (::MemoryStream*)_148->NativePtr;
    auto arg1 = (uint8*)buffer.ToPointer();
    ::StreamMemorySetRawBuffer(arg0, arg1);
}

Flood::Stream^ Flood::Stream::CreateWeb(Flood::Allocator^ alloc, System::String^ URL, Flood::StreamOpenMode mode)
{
    auto arg0 = (::Allocator*)alloc->NativePtr;
    auto arg1 = marshalString<E_UTF8>(URL);
    auto arg2 = (::StreamOpenMode)mode;
    auto ret = ::StreamCreateWeb(arg0, arg1, arg2);
    return gcnew Flood::Stream((::Stream*)ret);
}

Flood::MemoryStream::MemoryStream(::MemoryStream* native)
    : Stream(native)
{
}

Flood::MemoryStream::MemoryStream(System::IntPtr native)
    : Stream(native)
{
    auto __native = (::MemoryStream*)native.ToPointer();
}

