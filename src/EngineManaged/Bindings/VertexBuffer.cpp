/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "VertexBuffer.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace clix;

Flood::VertexElementP::VertexElementP(::VertexElementP* native)
{
    NativePtr = native;
}

Flood::VertexElementP::VertexElementP(System::IntPtr native)
{
    auto __native = (::VertexElementP*)native.ToPointer();
    NativePtr = __native;
}

Flood::VertexElement::VertexElement(::VertexElement* native)
    : VertexElementP(native)
{
}

Flood::VertexElement::VertexElement(System::IntPtr native)
    : VertexElementP(native)
{
    auto __native = (::VertexElement*)native.ToPointer();
}

Flood::VertexElement::VertexElement(Flood::VertexAttribute _188, Flood::VertexDataType _189, unsigned char components)
    : VertexElementP(nullptr)
{
    auto arg0 = (::VertexAttribute)_188;
    auto arg1 = (::VertexDataType)_189;
    auto arg2 = (uint8)components;
    NativePtr = new ::VertexElement(arg0, arg1, arg2);
}

unsigned char Flood::VertexElement::GetSize()
{
    auto ret = ((::VertexElement*)NativePtr)->getSize();
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

void Flood::VertexDeclaration::Add(Flood::VertexAttribute _191, int numComponents)
{
    auto arg0 = (::VertexAttribute)_191;
    ((::VertexDeclaration*)NativePtr)->add(arg0, numComponents);
}

void Flood::VertexDeclaration::Add(Flood::VertexElement^ _192)
{
    auto &arg0 = *(::VertexElement*)_192->NativePtr;
    ((::VertexDeclaration*)NativePtr)->add(arg0);
}

void Flood::VertexDeclaration::Add(Flood::VertexElementP^ _193)
{
    auto &arg0 = *(::VertexElementP*)_193->NativePtr;
    ((::VertexDeclaration*)NativePtr)->add(arg0);
}

void Flood::VertexDeclaration::Reset()
{
    ((::VertexDeclaration*)NativePtr)->reset();
}

Flood::VertexElement^ Flood::VertexDeclaration::Find(Flood::VertexAttribute _194)
{
    auto arg0 = (::VertexAttribute)_194;
    auto ret = ((::VertexDeclaration*)NativePtr)->find(arg0);
    return gcnew Flood::VertexElement((::VertexElement*)ret);
}

unsigned char Flood::VertexDeclaration::GetOffset(Flood::VertexAttribute _195)
{
    auto arg0 = (::VertexAttribute)_195;
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

