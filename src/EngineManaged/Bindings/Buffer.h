/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Graphics/Buffer.h>
#include "ResourceHandle.h"

namespace Flood
{
    enum struct BufferAccess : unsigned char;
    enum struct BufferUsage : unsigned char;
    ref class Buffer;
    ref class GeometryBuffer;

    /// <summary>
    /// "Static" means the data in VBO will not be changed (specified once and used
    /// many times), "dynamic" means the data will be changed frequently (specified
    /// and used repeatedly), and "stream" means the data will be changed every
    /// frame (specified once and used once). "Draw" means the data will be sent to
    /// GPU in order to draw (application to GL), "read" means the data will be
    /// read by the client's application (GL to application), and "copy" means the
    /// data will be used both drawing and reading (GL to GL).
    /// </summary>
    public enum struct BufferUsage : unsigned char
    {
        Static = 0,
        Stream = 1,
        Dynamic = 2
    };

    /// <summary>
    /// Use these enums to represent the lifetime and usage patterns of a buffer.
    /// These help the engine make better decisions about where to store the
    /// buffers, which leads to better rendering performance.
    /// </summary>
    public enum struct BufferAccess : unsigned char
    {
        Read = 0,
        Write = 1,
        ReadWrite = 2
    };

    /// <summary>
    /// Represents a buffer of data stored in memory. They mainly store rendering
    /// data given to the render API. The buffers may be stored in different kinds
    /// of memory, like system RAM or video RAM, depending on the usage and type
    /// flags that they are created with.
    /// </summary>
    public ref class Buffer
    {
    public:
        property ::Buffer* NativePtr;

        Buffer(::Buffer* native);
        Buffer(System::IntPtr native);
        Buffer();
        Buffer(Flood::BufferUsage usage, Flood::BufferAccess access);
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
        Flood::BufferUsage GetBufferUsage();
        void SetBufferUsage(Flood::BufferUsage v);
        Flood::BufferAccess GetBufferAccess();
        void SetBufferAccess(Flood::BufferAccess v);
        Flood::GeometryBuffer^ GetGeometryBuffer();
        void SetGeometryBuffer(Flood::GeometryBuffer^ v);
    };
}
