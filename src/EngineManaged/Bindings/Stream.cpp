/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "Stream.h"
#include "Memory.h"

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

Flood::Stream^ Flood::Stream::CreateFromURI(Flood::Allocator^ _135, System::String^ _136, Flood::StreamOpenMode _137)
{
    auto arg0 = _135->NativePtr;
    auto arg1 = marshalString<E_UTF8>(_136);
    auto arg2 = (::StreamOpenMode)_137;
    auto ret = ::StreamCreateFromURI(arg0, arg1, arg2);
    return gcnew Flood::Stream((::Stream*)ret);
}

void Flood::Stream::Destroy()
{
    auto arg0 = NativePtr;
    ::StreamDestroy(arg0);
}

bool Flood::Stream::Close()
{
    auto arg0 = NativePtr;
    auto ret = ::StreamClose(arg0);
    return ret;
}

long long Flood::Stream::GetSize()
{
    auto arg0 = NativePtr;
    auto ret = ::StreamGetSize(arg0);
    return ret;
}

void Flood::Stream::Resize(long long size)
{
    auto arg0 = NativePtr;
    auto arg1 = (int64)size;
    ::StreamResize(arg0, arg1);
}

long long Flood::Stream::ReadBuffer(System::IntPtr buffer, long long size)
{
    auto arg0 = NativePtr;
    auto arg1 = buffer.ToPointer();
    auto arg2 = (int64)size;
    auto ret = ::StreamReadBuffer(arg0, arg1, arg2);
    return ret;
}

long long Flood::Stream::ReadString(System::String^ _145)
{
    auto arg0 = NativePtr;
    auto arg1 = marshalString<E_UTF8>(_145);
    auto ret = ::StreamReadString(arg0, arg1);
    return ret;
}

long long Flood::Stream::Write(System::IntPtr buf, unsigned long long size)
{
    auto arg0 = NativePtr;
    auto arg1 = (uint8*)buf.ToPointer();
    auto arg2 = (uint64)size;
    auto ret = ::StreamWrite(arg0, arg1, arg2);
    return ret;
}

long long Flood::Stream::WriteString(System::String^ _150)
{
    auto arg0 = NativePtr;
    auto arg1 = marshalString<E_UTF8>(_150);
    auto ret = ::StreamWriteString(arg0, arg1);
    return ret;
}

long long Flood::Stream::GetPosition()
{
    auto arg0 = NativePtr;
    auto ret = ::StreamGetPosition(arg0);
    return ret;
}

long long Flood::Stream::SetPosition(long long _153, Flood::StreamSeekMode _154)
{
    auto arg0 = NativePtr;
    auto arg1 = (int64)_153;
    auto arg2 = (::StreamSeekMode)_154;
    auto ret = ::StreamSetPosition(arg0, arg1, arg2);
    return ret;
}

Flood::Stream^ Flood::Stream::CreateFromFile(Flood::Allocator^ _155, System::String^ _156, Flood::StreamOpenMode _157)
{
    auto arg0 = _155->NativePtr;
    auto arg1 = marshalString<E_UTF8>(_156);
    auto arg2 = (::StreamOpenMode)_157;
    auto ret = ::StreamCreateFromFile(arg0, arg1, arg2);
    return gcnew Flood::Stream((::Stream*)ret);
}

Flood::MemoryStream^ Flood::Stream::CreateFromMemory(Flood::Allocator^ _158, unsigned long long size)
{
    auto arg0 = _158->NativePtr;
    auto arg1 = (uint64)size;
    auto ret = ::StreamCreateFromMemory(arg0, arg1);
    return gcnew Flood::MemoryStream((::MemoryStream*)ret);
}

void Flood::Stream::MemoryInit(Flood::MemoryStream^ _159)
{
    auto arg0 = _159->NativePtr;
    ::StreamMemoryInit(arg0);
}

void Flood::Stream::MemorySetRawBuffer(Flood::MemoryStream^ _160, System::IntPtr buffer)
{
    auto arg0 = _160->NativePtr;
    auto arg1 = (uint8*)buffer.ToPointer();
    ::StreamMemorySetRawBuffer(arg0, arg1);
}

Flood::Stream^ Flood::Stream::CreateWeb(Flood::Allocator^ alloc, System::String^ URL, Flood::StreamOpenMode mode)
{
    auto arg0 = alloc->NativePtr;
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

