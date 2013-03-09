/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "VertexBuffer.h"
#include "ResourceHandle.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace clix;

Flood::VertexElementP::VertexElementP(::VertexElementP* native)
{
    Attribute = (Flood::VertexAttribute)native->attribute;
    Type = (Flood::VertexDataType)native->type;
    Components = native->components;
}

Flood::VertexElementP::VertexElementP(System::IntPtr native)
{
    auto __native = (::VertexElementP*)native.ToPointer();
    Attribute = (Flood::VertexAttribute)__native->attribute;
    Type = (Flood::VertexDataType)__native->type;
    Components = __native->components;
}

Flood::VertexElement::VertexElement(::VertexElement* native)
{
    Stride = native->stride;
    Offset = native->offset;
    Size = native->size;
}

Flood::VertexElement::VertexElement(System::IntPtr native)
{
    auto __native = (::VertexElement*)native.ToPointer();
    Stride = __native->stride;
    Offset = __native->offset;
    Size = __native->size;
}

Flood::VertexElement::VertexElement(Flood::VertexAttribute _173, Flood::VertexDataType _174, unsigned char components)
{
}

unsigned char Flood::VertexElement::GetSize()
{
    auto this0 = (::VertexElement*) 0;
    auto ret = this0->getSize();
    return ret;
}

Flood::VertexDeclaration::VertexDeclaration(::VertexDeclaration* native)
{
    NativePtr = native;
}

Flood::VertexDeclaration::VertexDeclaration(System::IntPtr native)
{
    auto __native = (::VertexDeclaration*)native.ToPointer();
    NativePtr = __native;
}

void Flood::VertexDeclaration::Add(Flood::VertexAttribute _176, int numComponents)
{
    auto arg0 = (::VertexAttribute)_176;
    ((::VertexDeclaration*)NativePtr)->add(arg0, numComponents);
}

void Flood::VertexDeclaration::Add(Flood::VertexElement _177)
{
    auto arg0 = ::VertexElement();
    arg0.stride = (int8)_177.Stride;
    arg0.offset = (uint32)_177.Offset;
    arg0.size = (uint32)_177.Size;

    ((::VertexDeclaration*)NativePtr)->add(arg0);
}

void Flood::VertexDeclaration::Add(Flood::VertexElementP _178)
{
    auto arg0 = ::VertexElementP();
    arg0.attribute = (::VertexAttribute)_178.Attribute;
    arg0.type = (::VertexDataType)_178.Type;
    arg0.components = (uint8)_178.Components;

    ((::VertexDeclaration*)NativePtr)->add(arg0);
}

void Flood::VertexDeclaration::Reset()
{
    ((::VertexDeclaration*)NativePtr)->reset();
}

Flood::VertexElement Flood::VertexDeclaration::Find(Flood::VertexAttribute _179)
{
    auto arg0 = (::VertexAttribute)_179;
    auto ret = ((::VertexDeclaration*)NativePtr)->find(arg0);
    return Flood::VertexElement((::VertexElement*)ret);
}

unsigned char Flood::VertexDeclaration::GetOffset(Flood::VertexAttribute _180)
{
    auto arg0 = (::VertexAttribute)_180;
    auto ret = ((::VertexDeclaration*)NativePtr)->getOffset(arg0);
    return ret;
}

unsigned char Flood::VertexDeclaration::GetVertexSize()
{
    auto ret = ((::VertexDeclaration*)NativePtr)->getVertexSize();
    return ret;
}

void Flood::VertexDeclaration::CalculateStrides()
{
    ((::VertexDeclaration*)NativePtr)->calculateStrides();
}

Flood::VertexBuffer::VertexBuffer(::VertexBuffer* native)
    : Buffer(native)
{
}

Flood::VertexBuffer::VertexBuffer(System::IntPtr native)
    : Buffer(native)
{
    auto __native = (::VertexBuffer*)native.ToPointer();
}

Flood::VertexBuffer::VertexBuffer()
    : Buffer(nullptr)
{
    NativePtr = new ::VertexBuffer();
}

bool Flood::VertexBuffer::IsBuilt()
{
    auto ret = ((::VertexBuffer*)NativePtr)->isBuilt();
    return ret;
}

void Flood::VertexBuffer::ForceRebuild()
{
    ((::VertexBuffer*)NativePtr)->forceRebuild();
}

