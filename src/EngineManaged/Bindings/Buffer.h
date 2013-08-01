/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "CppSharp.h"
#include <Graphics/Buffer.h>

namespace Flood
{
    enum struct BufferAccess : unsigned char;
    enum struct BufferUsage : unsigned char;
    ref class Buffer;
    ref class GeometryBuffer;

    public enum struct BufferUsage : unsigned char
    {
        Static = 0,
        Stream = 1,
        Dynamic = 2
    };

    public enum struct BufferAccess : unsigned char
    {
        Read = 0,
        Write = 1,
        ReadWrite = 2
    };

    public ref class Buffer : ICppInstance
    {
    public:
        property ::Buffer* NativePtr;
        property System::IntPtr Instance
        {
            virtual System::IntPtr get();
            virtual void set(System::IntPtr instance);
        }

        Buffer(::Buffer* native);
        Buffer(System::IntPtr native);
        Buffer();
        Buffer(Flood::BufferUsage usage, Flood::BufferAccess access);
        property Flood::BufferUsage BufferUsage
        {
            Flood::BufferUsage get();
            void set(Flood::BufferUsage);
        }
        property Flood::BufferAccess BufferAccess
        {
            Flood::BufferAccess get();
            void set(Flood::BufferAccess);
        }
        property Flood::GeometryBuffer^ GeometryBuffer
        {
            Flood::GeometryBuffer^ get();
            void set(Flood::GeometryBuffer^);
        }
        property unsigned int Id
        {
            unsigned int get();
            void set(unsigned int);
        }
        property Flood::BufferUsage Usage
        {
            Flood::BufferUsage get();
            void set(Flood::BufferUsage);
        }
        property Flood::BufferAccess Access
        {
            Flood::BufferAccess get();
            void set(Flood::BufferAccess);
        }
        property Flood::GeometryBuffer^ Gb
        {
            Flood::GeometryBuffer^ get();
            void set(Flood::GeometryBuffer^);
        }
        virtual bool Equals(System::Object^ object) override;
        virtual int GetHashCode() override;
    };
}
