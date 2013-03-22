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
    ref class Allocator;
    ref class FileStream;
    ref class MemoryStream;
    ref class scoped_ptr;
    ref class Stream;

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

    public delegate bool StreamOpenFunction(Flood::Stream^ _0);

    public delegate bool StreamCloseFunction(Flood::Stream^ _0);

    public delegate long long StreamReadFunction(Flood::Stream^ _1, System::IntPtr _2, long long _3);

    public delegate long long StreamWriteFunction(Flood::Stream^ _4, System::IntPtr _5, long long _6);

    public delegate long long StreamTellFunction(Flood::Stream^ _7);

    public delegate long long StreamSeekFunction(Flood::Stream^ _8, long long _9, char _10);

    public delegate long long StreamSizeFunction(Flood::Stream^ _11);

    public delegate void StreamResizeFunction(Flood::Stream^ _12, long long size);

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
        property System::String^ Path
        {
            System::String^ get();
            void set(System::String^);
        }
        property Flood::StreamOpenMode Mode
        {
            Flood::StreamOpenMode get();
            void set(Flood::StreamOpenMode);
        }
        void Destroy();
        bool Close();
        long long GetSize();
        void Resize(long long size);
        long long ReadBuffer(System::IntPtr buffer, long long size);
        long long ReadString(System::String^ _1);
        long long Write(System::IntPtr buf, unsigned long long size);
        long long WriteString(System::String^ _1);
        long long GetPosition();
        long long SetPosition(long long _1, Flood::StreamSeekMode _2);
        static Flood::Stream^ CreateFromURI(Flood::Allocator^ _0, System::String^ _1, Flood::StreamOpenMode _2);
        static Flood::Stream^ CreateFromFile(Flood::Allocator^ _0, System::String^ _1, Flood::StreamOpenMode _2);
        static Flood::MemoryStream^ CreateFromMemory(Flood::Allocator^ _1, unsigned long long size);
        static void MemoryInit(Flood::MemoryStream^ _0);
        static void MemorySetRawBuffer(Flood::MemoryStream^ _0, System::IntPtr buffer);
        static Flood::Stream^ CreateWeb(Flood::Allocator^ alloc, System::String^ URL, Flood::StreamOpenMode mode);
    };

    public ref class MemoryStream : Flood::Stream
    {
    public:
        MemoryStream(::MemoryStream* native);
        MemoryStream(System::IntPtr native);
        property System::IntPtr Buffer
        {
            System::IntPtr get();
            void set(System::IntPtr);
        }
        property unsigned long long Position
        {
            unsigned long long get();
            void set(unsigned long long);
        }
        property bool UseRawBuffer
        {
            bool get();
            void set(bool);
        }
    };
}
