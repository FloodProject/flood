/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "CppSharp.h"
#include <Core/Stream.h>
#include "StringConverter.h"

namespace Flood
{
    enum struct StreamOpenMode;
    enum struct StreamSeekMode;
    ref class Stream;
    ref class WebStream;
    ref class ZipStream;
    ref class Zzip_dir;
    ref class Zzip_file;
}

namespace Flood
{
    /// <summary>
    /// Use these for different kinds of access to the streams.
    /// </summary>
    public enum struct StreamOpenMode
    {
        Read = 0,
        Write = 1,
        Append = 2,
        Default = 3
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

    /// <summary>
    /// A stream allows uniform access to data backed by different storage mediums
    /// like memory, files, archives, or even in remove servers.
    /// </summary>
    public ref class Stream : ICppInstance
    {
    public:

        enum struct _0
        {
            EndOfStream = 0,
            InvalidState = -1
        };

        property ::Stream* NativePtr;
        property System::IntPtr __Instance
        {
            virtual System::IntPtr get();
            virtual void set(System::IntPtr instance);
        }

        Stream(::Stream* native);
        Stream(System::IntPtr native);
        property unsigned long long Position
        {
            unsigned long long get();
        }

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

        /// <summary>
        /// Opens the stream.
        /// </summary>
        virtual bool Open();

        /// <summary>
        /// Closes the stream.
        /// </summary>
        virtual bool Close();

        /// <summary>
        /// Reads from the the stream into a buffer.
        /// </summary>
        virtual unsigned long long Read(void* buffer, unsigned long long size);

        /// <summary>
        /// Writes from buffer into the the stream.
        /// </summary>
        virtual unsigned long long Write(void* buffer, unsigned long long size);

        /// <summary>
        /// Set stream position.
        /// </summary>
        virtual void SetPosition(long long pos, Flood::StreamSeekMode mode);

        /// <summary>
        /// Get stream size.
        /// </summary>
        virtual unsigned long long Size();

        /// <summary>
        /// Resize stream.
        /// </summary>
        virtual void Resize(long long size);

        /// <summary>
        /// Reads from the the stream into a byte vector.
        /// </summary>
        unsigned long long Read(System::Collections::Generic::List<unsigned char>^ data);

        /// <summary>
        /// Reads from the the stream into a buffer.
        /// </summary>
        unsigned long long ReadBuffer(void* buffer, long long size);

        /// <summary>
        /// Reads from the the stream into a string.
        /// </summary>
        unsigned long long ReadString(System::String^ text);

        /// <summary>
        /// Reads from the the stream into a string.
        /// </summary>
        unsigned long long ReadUTF8String(System::String^ text);

        /// <summary>
        /// Reads from the the stream into a list of lines.
        /// </summary>
        unsigned long long ReadLines(System::Collections::Generic::List<System::String^>^ lines);

        /// <summary>
        /// Reads from the the stream into a list of lines.
        /// </summary>
        unsigned long long ReadUTF8Lines(System::Collections::Generic::List<System::String^>^ lines);

        /// <summary>
        /// Writes from buffer into the the stream.
        /// </summary>
        unsigned long long Write(unsigned char* buf, unsigned long long size);

        /// <summary>
        /// Writes from string into the the stream.
        /// </summary>
        unsigned long long WriteString(System::String^ string);

        /// <summary>
        /// Writes from utf8 string into the the stream.
        /// </summary>
        unsigned long long WriteUTF8String(System::String^ string);

        virtual bool Equals(System::Object^ object) override;

        virtual int GetHashCode() override;
    };

    public ref class WebStream : Flood::Stream
    {
    public:

        WebStream(::WebStream* native);
        WebStream(System::IntPtr native);
        /// <summary>
        /// Creates Web stream.
        /// </summary>
        WebStream(System::String^ URL, Flood::StreamOpenMode mode);

        property void* Handle
        {
            void* get();
            void set(void*);
        }

        property bool IsPerformDone
        {
            bool get();
            void set(bool);
        }

        /// <summary>
        /// Opens the stream.
        /// </summary>
        virtual bool Open() override;

        /// <summary>
        /// Closes the stream.
        /// </summary>
        virtual bool Close() override;

        /// <summary>
        /// Reads from the the stream into a buffer.
        /// </summary>
        virtual unsigned long long Read(void* buffer, unsigned long long size) override;

        /// <summary>
        /// Writes from buffer into the the stream.
        /// </summary>
        virtual unsigned long long Write(void* buffer, unsigned long long size) override;

        /// <summary>
        /// Get stream size.
        /// </summary>
        virtual unsigned long long Size() override;

        /// <summary>
        /// Update memory stream from current url content.
        /// </summary>
        bool Perform();

        virtual bool Equals(System::Object^ object) override;

        virtual int GetHashCode() override;
    };

    public ref class ZipStream : Flood::Stream
    {
    public:

        ZipStream(::ZipStream* native);
        ZipStream(System::IntPtr native);
        property unsigned long long Position
        {
            unsigned long long get();
        }

        /// <summary>
        /// Opens the stream.
        /// </summary>
        virtual bool Open() override;

        /// <summary>
        /// Closes the stream.
        /// </summary>
        virtual bool Close() override;

        /// <summary>
        /// Reads from the the stream into a buffer.
        /// </summary>
        virtual unsigned long long Read(void* buffer, unsigned long long size) override;

        /// <summary>
        /// Set stream position.
        /// </summary>
        virtual void SetPosition(long long pos, Flood::StreamSeekMode mode) override;

        /// <summary>
        /// Get stream size.
        /// </summary>
        virtual unsigned long long Size() override;

        virtual bool Equals(System::Object^ object) override;

        virtual int GetHashCode() override;
    };
}
