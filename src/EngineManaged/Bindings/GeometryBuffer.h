/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "CppSharp.h"
#include <Graphics/GeometryBuffer.h>

namespace Flood
{
    enum struct BufferAccess : unsigned char;
    enum struct BufferUsage : unsigned char;
    enum struct VertexAttribute : unsigned char;
    ref class GeometryBuffer;
    ref class VertexDeclaration;
}

namespace Flood
{
    /// <summary>
    /// Represents a buffer with geometry data. You have to associate the vertex
    /// data layout to the buffer so it can be used by the engine.
    /// </summary>
    public ref class GeometryBuffer : ICppInstance
    {
    public:

        property ::GeometryBuffer* NativePtr;
        property System::IntPtr __Instance
        {
            virtual System::IntPtr get();
            virtual void set(System::IntPtr instance);
        }

        GeometryBuffer(::GeometryBuffer* native);
        GeometryBuffer(System::IntPtr native);
        GeometryBuffer();

        GeometryBuffer(Flood::BufferUsage _0, Flood::BufferAccess _1);

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

        property unsigned int NumVertices
        {
            unsigned int get();
        }

        property unsigned int NumIndices
        {
            unsigned int get();
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

        void ForceRebuild();

        void Clear();

        void Set(Flood::VertexAttribute _0, unsigned char* data, unsigned int size);

        void Set(unsigned char* data, unsigned int size);

        void Add(unsigned char* data, unsigned int size);

        void SetIndex(unsigned char* data, unsigned int size);

        void AddIndex(unsigned char* data, unsigned int size);

        void AddIndex(unsigned short index);

        bool IsIndexed();

        float* GetAttribute(Flood::VertexAttribute _0, unsigned int i);

        char GetAttributeStride(Flood::VertexAttribute _0);

        void ClearIndexes();

        virtual bool Equals(System::Object^ object) override;

        virtual int GetHashCode() override;
    };
}
