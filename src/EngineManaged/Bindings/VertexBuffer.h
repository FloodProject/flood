/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Graphics/VertexBuffer.h>
#include "Buffer.h"

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

    public ref class VertexElement : VertexElementP
    {
    public:
        VertexElement(::VertexElement* native);
        VertexElement(System::IntPtr native);
        VertexElement(Flood::VertexAttribute _188, Flood::VertexDataType _189, unsigned char components);
        property char Stride;
        property unsigned int Offset;
        property unsigned int Size;
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
        void Add(Flood::VertexAttribute _191, int numComponents);
        void Add(Flood::VertexElement^ _192);
        void Add(Flood::VertexElementP^ _193);
        void Reset();
        Flood::VertexElement^ Find(Flood::VertexAttribute _194);
        unsigned char GetOffset(Flood::VertexAttribute _195);
        unsigned char GetVertexSize();
        void CalculateStrides();
    };

    public ref class VertexBuffer : Buffer
    {
    public:
        VertexBuffer(::VertexBuffer* native);
        VertexBuffer(System::IntPtr native);
        VertexBuffer();
        property bool Built;
        bool IsBuilt();
        void ForceRebuild();
    };
}
