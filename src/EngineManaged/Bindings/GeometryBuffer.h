/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Graphics/GeometryBuffer.h>
#include "Buffer.h"
#include "ResourceHandle.h"
#include "VertexBuffer.h"

namespace Flood
{
    enum struct BufferAccess : unsigned char;
    enum struct BufferUsage : unsigned char;
    enum struct VertexAttribute : unsigned char;
    ref class GeometryBuffer;
    ref class VertexDeclaration;

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
        GeometryBuffer(Flood::BufferUsage _0, Flood::BufferAccess _1);
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
        property System::Collections::Generic::List<unsigned char>^ Data
        {
            System::Collections::Generic::List<unsigned char>^ get();
            void set(System::Collections::Generic::List<unsigned char>^);
        }
        property System::Collections::Generic::List<unsigned char>^ IndexData
        {
            System::Collections::Generic::List<unsigned char>^ get();
            void set(System::Collections::Generic::List<unsigned char>^);
        }
        property unsigned char IndexSize
        {
            unsigned char get();
            void set(unsigned char);
        }
        property bool NeedsRebuild
        {
            bool get();
            void set(bool);
        }
        property unsigned int Hash
        {
            unsigned int get();
            void set(unsigned int);
        }
        property Flood::VertexDeclaration^ Declarations
        {
            Flood::VertexDeclaration^ get();
            void set(Flood::VertexDeclaration^);
        }
        Flood::BufferUsage GetBufferUsage();
        void SetBufferUsage(Flood::BufferUsage v);
        Flood::BufferAccess GetBufferAccess();
        void SetBufferAccess(Flood::BufferAccess v);
        void ForceRebuild();
        void Clear();
        void Set(Flood::VertexAttribute _0, System::IntPtr data, unsigned int size);
        void Set(System::IntPtr data, unsigned int size);
        void Add(System::IntPtr data, unsigned int size);
        void SetIndex(System::IntPtr data, unsigned int size);
        void AddIndex(System::IntPtr data, unsigned int size);
        void AddIndex(unsigned short index);
        bool IsIndexed();
        System::IntPtr GetAttribute(Flood::VertexAttribute _0, unsigned int i);
        char GetAttributeStride(Flood::VertexAttribute _0);
        unsigned int GetNumVertices();
        unsigned int GetNumIndices();
        void ClearIndexes();
    };
}
