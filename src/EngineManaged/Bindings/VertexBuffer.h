/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "CppSharp.h"
#include <Graphics/VertexBuffer.h>
#include "Buffer.h"

namespace Flood
{
    enum struct VertexAttribute : unsigned char;
    enum struct VertexDataType : unsigned char;
    ref class VertexBuffer;
    ref class VertexDeclaration;
    value struct VertexElement;
    value struct VertexElementP;
}

namespace Flood
{
    /// <summary>
    /// Attribute of a vertex element.
    /// </summary>
    public enum struct VertexAttribute : unsigned char
    {
        Position = 0,
        Normal = 2,
        Color = 3,
        BoneIndex = 4,
        FogCoord = 5,
        TexCoord0 = 6,
        TexCoord1 = 7,
        TexCoord2 = 8,
        TexCoord3 = 9,
        TexCoord4 = 10,
        TexCoord5 = 11,
        TexCoord6 = 12,
        TexCoord7 = 13
    };

    public enum struct VertexDataType : unsigned char
    {
        Byte = 0,
        Float = 1,
        Integer = 2
    };

    /// <summary>
    /// Each element inside a vertex declaration.
    /// </summary>
    public value struct VertexElementP
    {
    public:

        VertexElementP(::VertexElementP* native);
        VertexElementP(System::IntPtr native);
        property Flood::VertexAttribute Attribute
        {
            Flood::VertexAttribute get();
            void set(Flood::VertexAttribute);
        }

        property Flood::VertexDataType Type
        {
            Flood::VertexDataType get();
            void set(Flood::VertexDataType);
        }

        property unsigned char Components
        {
            unsigned char get();
            void set(unsigned char);
        }

        private:
        Flood::VertexAttribute __Attribute;
        Flood::VertexDataType __Type;
        unsigned char __Components;
    };

    public value struct VertexElement
    {
    public:

        VertexElement(::VertexElement* native);
        VertexElement(System::IntPtr native);
        VertexElement(Flood::VertexAttribute _0, Flood::VertexDataType _1, unsigned char components);

        property Flood::VertexAttribute Attribute
        {
            Flood::VertexAttribute get();
            void set(Flood::VertexAttribute);
        }

        property Flood::VertexDataType Type
        {
            Flood::VertexDataType get();
            void set(Flood::VertexDataType);
        }

        property unsigned char Components
        {
            unsigned char get();
            void set(unsigned char);
        }

        property unsigned char Size
        {
            unsigned char get();
        }

        property char Stride
        {
            char get();
            void set(char);
        }

        property unsigned int Offset
        {
            unsigned int get();
            void set(unsigned int);
        }

        property unsigned int Size1
        {
            unsigned int get();
            void set(unsigned int);
        }

        private:
        Flood::VertexAttribute __Attribute;
        Flood::VertexDataType __Type;
        unsigned char __Components;
        char __Stride;
        unsigned int __Offset;
        unsigned int __Size;
    };

    /// <summary>
    /// This describes structure of a geometry buffer.
    /// </summary>
    public ref class VertexDeclaration : ICppInstance
    {
    public:

        property ::VertexDeclaration* NativePtr;
        property System::IntPtr __Instance
        {
            virtual System::IntPtr get();
            virtual void set(System::IntPtr instance);
        }

        VertexDeclaration(::VertexDeclaration* native);
        VertexDeclaration(System::IntPtr native);
        VertexDeclaration();

        property unsigned char VertexSize
        {
            unsigned char get();
        }

        property System::Collections::Generic::List<Flood::VertexElement>^ Decls
        {
            System::Collections::Generic::List<Flood::VertexElement>^ get();
            void set(System::Collections::Generic::List<Flood::VertexElement>^);
        }

        void Add(Flood::VertexAttribute _0, int numComponents);

        void Add(Flood::VertexElement _0);

        void Add(Flood::VertexElementP _0);

        void Reset();

        Flood::VertexElement Find(Flood::VertexAttribute _0);

        unsigned char GetOffset(Flood::VertexAttribute _0);

        void CalculateStrides();

        virtual bool Equals(System::Object^ object) override;

        virtual int GetHashCode() override;
    };

    public ref class VertexBuffer : Flood::Buffer
    {
    public:

        VertexBuffer(::VertexBuffer* native);
        VertexBuffer(System::IntPtr native);
        VertexBuffer();

        property bool Built
        {
            bool get();
            void set(bool);
        }

        bool IsBuilt();

        void ForceRebuild();

        virtual bool Equals(System::Object^ object) override;

        virtual int GetHashCode() override;
    };
}
