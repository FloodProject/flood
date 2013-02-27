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

Flood::VertexElement::VertexElement(Flood::VertexAttribute _185, Flood::VertexDataType _186, unsigned char components)
    : VertexElementP(nullptr)
{
    auto arg0 = (::VertexAttribute)_185;
    auto arg1 = (::VertexDataType)_186;
    auto arg2 = (uint8)components;
    NativePtr = new ::VertexElement(arg0, arg1, arg2);
}

unsigned char Flood::VertexElement::GetSize()
{
    auto ret = NativePtr->getSize();
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

void Flood::VertexDeclaration::Add(Flood::VertexAttribute _188, int numComponents)
{
    auto arg0 = (::VertexAttribute)_188;
    NativePtr->add(arg0, numComponents);
}

void Flood::VertexDeclaration::Add(Flood::VertexElement^ _189)
{
    auto arg0 = *_189->NativePtr;
    NativePtr->add(arg0);
}

void Flood::VertexDeclaration::Add(Flood::VertexElementP^ _190)
{
    auto arg0 = *_190->NativePtr;
    NativePtr->add(arg0);
}

void Flood::VertexDeclaration::Reset()
{
    NativePtr->reset();
}

Flood::VertexElement^ Flood::VertexDeclaration::Find(Flood::VertexAttribute _191)
{
    auto arg0 = (::VertexAttribute)_191;
    auto ret = NativePtr->find(arg0);
    return gcnew Flood::VertexElement((::VertexElement*)ret);
}

unsigned char Flood::VertexDeclaration::GetOffset(Flood::VertexAttribute _192)
{
    auto arg0 = (::VertexAttribute)_192;
    auto ret = NativePtr->getOffset(arg0);
    return ret;
}

unsigned char Flood::VertexDeclaration::GetVertexSize()
{
    auto ret = NativePtr->getVertexSize();
    return ret;
}

void Flood::VertexDeclaration::CalculateStrides()
{
    NativePtr->calculateStrides();
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
    auto ret = NativePtr->isBuilt();
    return ret;
}

void Flood::VertexBuffer::ForceRebuild()
{
    NativePtr->forceRebuild();
}

