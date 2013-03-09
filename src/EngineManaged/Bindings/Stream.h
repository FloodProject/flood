/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Core/Stream.h>
#include "ResourceHandle.h"

namespace Flood
{
    enum struct StreamOpenMode;
    enum struct StreamSeekMode;
    ref class Stream;
    ref class Allocator;
    ref class MemoryStream;
    ref class FileStream;
    ref class scoped_ptr;

    /// <summary>
    /// Use these for different kinds of access to the streams.
    /// </summary>
    public enum struct StreamOpenMode
    {
        Read = 0,
        Write = 1,
        Append = 2
    };

    /// <summary>
    /// Controls where the seeking will occur from.
    /// </summary>
    public enum struct StreamSeekMode
    {
        Absolute = 0,
        Relative = 1,
        RelativeEnd = 2
    };

    public delegate bool StreamOpenFunction(Flood::Stream^);

    public delegate bool StreamCloseFunction(Flood::Stream^);

    public delegate long long StreamReadFunction(Flood::Stream^ ,System::IntPtr ,long long);

    public delegate long long StreamWriteFunction(Flood::Stream^ ,System::IntPtr ,long long);

    public delegate long long StreamTellFunction(Flood::Stream^);

    public delegate long long StreamSeekFunction(Flood::Stream^ ,long long ,char);

    public delegate long long StreamSizeFunction(Flood::Stream^);

    public delegate void StreamResizeFunction(Flood::Stream^ ,long long size);

    /// <summary>
    /// A stream allows uniform access to data backed by different storage mediums
    /// like memory, files, archives, or even in remove servers.
    /// </summary>
    public ref class Stream
    {
    public:
        property ::Stream* NativePtr;

        Stream(::Stream* native);
        Stream(System::IntPtr native);
        property System::String^ Path;
        property Flood::StreamOpenMode Mode;
        void Destroy();
        bool Close();
        long long GetSize();
        void Resize(long long size);
        long long ReadBuffer(System::IntPtr buffer, long long size);
        long long ReadString(System::String^ _133);
        long long Write(System::IntPtr buf, unsigned long long size);
        long long WriteString(System::String^ _138);
        long long GetPosition();
        long long SetPosition(long long _141, Flood::StreamSeekMode _142);
        static Flood::Stream^ CreateFromURI(Flood::Allocator^ _123, System::String^ _124, Flood::StreamOpenMode _125);
        static Flood::Stream^ CreateFromFile(Flood::Allocator^ _143, System::String^ _144, Flood::StreamOpenMode _145);
        static Flood::MemoryStream^ CreateFromMemory(Flood::Allocator^ _146, unsigned long long size);
        static void MemoryInit(Flood::MemoryStream^ _147);
        static void MemorySetRawBuffer(Flood::MemoryStream^ _148, System::IntPtr buffer);
        static Flood::Stream^ CreateWeb(Flood::Allocator^ alloc, System::String^ URL, Flood::StreamOpenMode mode);
    };

    public ref class MemoryStream : Flood::Stream
    {
    public:
        MemoryStream(::MemoryStream* native);
        MemoryStream(System::IntPtr native);
        property System::IntPtr Buffer;
        property unsigned long long Position;
        property bool UseRawBuffer;
    };
}
