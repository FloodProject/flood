/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Graphics/VertexBuffer.h>

namespace Flood
{
    enum struct VertexAttribute : unsigned char;
    enum struct VertexDataType : unsigned char;
    ref class VertexElementP;
    ref class VertexElement;
    ref class VertexDeclaration;
    ref class VertexBuffer;
    ref class RefPtr;

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
    public ref class VertexElementP
    {
    public:
        property ::VertexElementP* NativePtr;

        VertexElementP(::VertexElementP* native);
        VertexElementP(System::IntPtr native);
        property Flood::VertexAttribute Attribute;
        property Flood::VertexDataType Type;
        property unsigned char Components;
    };

    public ref class VertexElement
    {
    public:
        property ::VertexElement* NativePtr;

        VertexElement(::VertexElement* native);
        VertexElement(System::IntPtr native);
        property char Stride;
        property unsigned int Offset;
        property unsigned int Size;
        VertexElement(Flood::VertexAttribute _185, Flood::VertexDataType _186, unsigned char components);
        unsigned char GetSize();
    };

    /// <summary>
    /// This describes structure of a geometry buffer.
    /// </summary>
    public ref class VertexDeclaration
    {
    public:
        property ::VertexDeclaration* NativePtr;

        VertexDeclaration(::VertexDeclaration* native);
        VertexDeclaration(System::IntPtr native);
        void Add(Flood::VertexAttribute _188, int numComponents);
        void Add(Flood::VertexElement^ _189);
        void Add(Flood::VertexElementP^ _190);
        void Reset();
        Flood::VertexElement^ Find(Flood::VertexAttribute _191);
        unsigned char GetOffset(Flood::VertexAttribute _192);
        unsigned char GetVertexSize();
        void CalculateStrides();
    };

    public ref class VertexBuffer
    {
    public:
        property ::VertexBuffer* NativePtr;

        VertexBuffer(::VertexBuffer* native);
        VertexBuffer(System::IntPtr native);
        property bool Built;
        VertexBuffer();
        bool IsBuilt();
        void ForceRebuild();
    };
}
