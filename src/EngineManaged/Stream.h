/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Core/Stream.h>

namespace Flood
{
    enum struct StreamOpenMode;
    enum struct StreamSeekMode;
    ref class Stream;
    ref class Allocator;
    ref class MemoryStream;
    ref class StreamFuncs;
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

    delegate bool StreamOpenFunction(Flood::Stream^);

    delegate bool StreamCloseFunction(Flood::Stream^);

    delegate long StreamReadFunction(Flood::Stream^, System::IntPtr, long);

    delegate long StreamWriteFunction(Flood::Stream^, System::IntPtr, long);

    delegate long StreamTellFunction(Flood::Stream^);

    delegate long StreamSeekFunction(Flood::Stream^, long, char);

    delegate long StreamSizeFunction(Flood::Stream^);

    delegate void StreamResizeFunction(Flood::Stream^, long size);


    /// <summary>
    /// Streams can be created from URIs (Univeral Resource Identifiers). Each
    /// stream protocol supported has a unique scheme that will be used to
    /// identify it when opening the stream. Examples, http:, or zip: would be
    /// valid schemes for an HTTP or ZIP stream.
    /// </summary>
    public ref class Stream
    {
        property ::Stream* NativePtr;

    public:
        Stream(::Stream* native);
    };

    public ref class FileStream
    {
        property ::FileStream* NativePtr;

    public:
        FileStream(::FileStream* native);
    };

    public ref class MemoryStream
    {
        property ::MemoryStream* NativePtr;

    public:
        MemoryStream(::MemoryStream* native);
    };

    public ref class FloodStream
    {
    public:
        static Flood::Stream^ StreamCreateFromURI (Flood::Allocator^ _95, System::String^ _96, Flood::StreamOpenMode _97);
        static void StreamDestroy (Flood::Stream^ _98);
        static bool StreamClose (Flood::Stream^ _99);
        static long StreamGetSize (Flood::Stream^ _100);
        static void StreamResize (Flood::Stream^ _101, long size);
        static long StreamRead (Flood::Stream^ _102, System::Collections::Generic::List<unsigned char>^ data);
        static long StreamReadBuffer (Flood::Stream^ _103, System::IntPtr buffer, long size);
        static long StreamReadString (Flood::Stream^ _104, System::String^ _105);
        static long StreamReadLines (Flood::Stream^ _106, System::Collections::Generic::List<System::String^>^ _107);
        static long StreamWrite (Flood::Stream^ _108, unsigned char buf, unsigned long size);
        static long StreamWriteString (Flood::Stream^ _109, System::String^ _110);
        static long StreamGetPosition (Flood::Stream^ _111);
        static long StreamSetPosition (Flood::Stream^ _112, long _113, Flood::StreamSeekMode _114);
        static Flood::Stream^ StreamCreateFromFile (Flood::Allocator^ _115, System::String^ _116, Flood::StreamOpenMode _117);
        static Flood::MemoryStream^ StreamCreateFromMemory (Flood::Allocator^ _118, unsigned long size);
        static void StreamMemoryInit (Flood::MemoryStream^ _119);
        static void StreamMemorySetRawBuffer (Flood::MemoryStream^ _120, unsigned char buffer);
        static Flood::Stream^ StreamCreateWeb (Flood::Allocator^ alloc, System::String^ URL, Flood::StreamOpenMode mode);
    };
}

