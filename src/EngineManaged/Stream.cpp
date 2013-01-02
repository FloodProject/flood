/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Stream.h"
#include "Memory.h"

using namespace System;
using namespace System::Runtime::InteropServices;

Flood::Stream::Stream(::Stream* native)
{
    NativePtr = native;
}

Flood::FileStream::FileStream(::FileStream* native)
{
    NativePtr = native;
}

Flood::MemoryStream::MemoryStream(::MemoryStream* native)
{
    NativePtr = native;
}

Flood::Stream^ Flood::FloodStream::StreamCreateFromURI(Flood::Allocator^ _95, System::String^ _96, Flood::StreamOpenMode _97)
{
    return nullptr;
}

void Flood::FloodStream::StreamDestroy(Flood::Stream^ _98)
{
}

bool Flood::FloodStream::StreamClose(Flood::Stream^ _99)
{
    return false;
}

long Flood::FloodStream::StreamGetSize(Flood::Stream^ _100)
{
    return 0;
}

void Flood::FloodStream::StreamResize(Flood::Stream^ _101, long size)
{
}

long Flood::FloodStream::StreamRead(Flood::Stream^ _102, System::Collections::Generic::List<unsigned char>^ data)
{
    return 0;
}

long Flood::FloodStream::StreamReadBuffer(Flood::Stream^ _103, System::IntPtr buffer, long size)
{
    return 0;
}

long Flood::FloodStream::StreamReadString(Flood::Stream^ _104, System::String^ _105)
{
    return 0;
}

long Flood::FloodStream::StreamReadLines(Flood::Stream^ _106, System::Collections::Generic::List<System::String^>^ _107)
{
    return 0;
}

long Flood::FloodStream::StreamWrite(Flood::Stream^ _108, unsigned char buf, unsigned long size)
{
    return 0;
}

long Flood::FloodStream::StreamWriteString(Flood::Stream^ _109, System::String^ _110)
{
    return 0;
}

long Flood::FloodStream::StreamGetPosition(Flood::Stream^ _111)
{
    return 0;
}

long Flood::FloodStream::StreamSetPosition(Flood::Stream^ _112, long _113, Flood::StreamSeekMode _114)
{
    return 0;
}

Flood::Stream^ Flood::FloodStream::StreamCreateFromFile(Flood::Allocator^ _115, System::String^ _116, Flood::StreamOpenMode _117)
{
    return nullptr;
}

Flood::MemoryStream^ Flood::FloodStream::StreamCreateFromMemory(Flood::Allocator^ _118, unsigned long size)
{
    return nullptr;
}

void Flood::FloodStream::StreamMemoryInit(Flood::MemoryStream^ _119)
{
}

void Flood::FloodStream::StreamMemorySetRawBuffer(Flood::MemoryStream^ _120, unsigned char buffer)
{
}

Flood::Stream^ Flood::FloodStream::StreamCreateWeb(Flood::Allocator^ alloc, System::String^ URL, Flood::StreamOpenMode mode)
{
    return nullptr;
}


