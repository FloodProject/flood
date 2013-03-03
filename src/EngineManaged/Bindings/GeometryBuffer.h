/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Graphics/GeometryBuffer.h>

namespace Flood
{
    ref class GeometryBuffer;
    enum struct BufferUsage : unsigned char;
    enum struct BufferAccess : unsigned char;
    ref class VertexDeclaration;
    enum struct VertexAttribute : unsigned char;
    ref class RefPtr;

    /// <summary>
    /// Represents a buffer with geometry data. You have to associate the vertex
    /// data layout to the buffer so it can be used by the engine.
    /// </summary>
    public ref class GeometryBuffer
    {
    public:
        property ::GeometryBuffer* NativePtr;

        GeometryBuffer(::GeometryBuffer* native);
        GeometryBuffer(System::IntPtr native);
        GeometryBuffer();
        GeometryBuffer(Flood::BufferUsage _183, Flood::BufferAccess _184);
        property Flood::BufferUsage Usage;
        property Flood::BufferAccess Access;
        property unsigned char IndexSize;
        property bool NeedsRebuild;
        property unsigned int Hash;
        property Flood::VertexDeclaration^ Declarations;
        Flood::BufferUsage GetBufferUsage();
        void SetBufferUsage(Flood::BufferUsage v);
        Flood::BufferAccess GetBufferAccess();
        void SetBufferAccess(Flood::BufferAccess v);
        void ForceRebuild();
        void Clear();
        void Set(Flood::VertexAttribute _185, System::IntPtr data, unsigned int size);
        void Set(System::IntPtr data, unsigned int size);
        void Add(System::IntPtr data, unsigned int size);
        void SetIndex(System::IntPtr data, unsigned int size);
        void AddIndex(System::IntPtr data, unsigned int size);
        void AddIndex(unsigned short index);
        bool IsIndexed();
        float GetAttribute(Flood::VertexAttribute _186, unsigned int i);
        char GetAttributeStride(Flood::VertexAttribute _187);
        unsigned int GetNumVertices();
        unsigned int GetNumIndices();
    };
}
