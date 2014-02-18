/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "GeometryBuffer.h"
#include "Buffer.h"
#include "VertexBuffer.h"

using namespace System;
using namespace System::Runtime::InteropServices;

Flood::GeometryBuffer::GeometryBuffer(::GeometryBuffer* native)
{
    NativePtr = native;
}

Flood::GeometryBuffer::GeometryBuffer(System::IntPtr native)
{
    auto __native = (::GeometryBuffer*)native.ToPointer();
    NativePtr = __native;
}

Flood::GeometryBuffer::GeometryBuffer()
{
    NativePtr = new ::GeometryBuffer();
}

Flood::GeometryBuffer::GeometryBuffer(Flood::BufferUsage _0, Flood::BufferAccess _1)
{
    auto arg0 = (::BufferUsage)_0;
    auto arg1 = (::BufferAccess)_1;
    NativePtr = new ::GeometryBuffer(arg0, arg1);
}

void Flood::GeometryBuffer::ForceRebuild()
{
    ((::GeometryBuffer*)NativePtr)->forceRebuild();
}

void Flood::GeometryBuffer::Clear()
{
    ((::GeometryBuffer*)NativePtr)->clear();
}

void Flood::GeometryBuffer::Set(Flood::VertexAttribute _0, System::IntPtr data, unsigned int size)
{
    auto arg0 = (::VertexAttribute)_0;
    auto arg1 = (::uint8*)data.ToPointer();
    auto arg2 = (::uint32)(::uint32_t)size;
    ((::GeometryBuffer*)NativePtr)->set(arg0, arg1, arg2);
}

void Flood::GeometryBuffer::Set(System::IntPtr data, unsigned int size)
{
    auto arg0 = (::uint8*)data.ToPointer();
    auto arg1 = (::uint32)(::uint32_t)size;
    ((::GeometryBuffer*)NativePtr)->set(arg0, arg1);
}

void Flood::GeometryBuffer::Add(System::IntPtr data, unsigned int size)
{
    auto arg0 = (::uint8*)data.ToPointer();
    auto arg1 = (::uint32)(::uint32_t)size;
    ((::GeometryBuffer*)NativePtr)->add(arg0, arg1);
}

void Flood::GeometryBuffer::SetIndex(System::IntPtr data, unsigned int size)
{
    auto arg0 = (::uint8*)data.ToPointer();
    auto arg1 = (::uint32)(::uint32_t)size;
    ((::GeometryBuffer*)NativePtr)->setIndex(arg0, arg1);
}

void Flood::GeometryBuffer::AddIndex(System::IntPtr data, unsigned int size)
{
    auto arg0 = (::uint8*)data.ToPointer();
    auto arg1 = (::uint32)(::uint32_t)size;
    ((::GeometryBuffer*)NativePtr)->addIndex(arg0, arg1);
}

void Flood::GeometryBuffer::AddIndex(unsigned short index)
{
    auto arg0 = (::uint16)(::uint16_t)index;
    ((::GeometryBuffer*)NativePtr)->addIndex(arg0);
}

bool Flood::GeometryBuffer::IsIndexed()
{
    auto __ret = ((::GeometryBuffer*)NativePtr)->isIndexed();
    return __ret;
}

System::IntPtr Flood::GeometryBuffer::GetAttribute(Flood::VertexAttribute _0, unsigned int i)
{
    auto arg0 = (::VertexAttribute)_0;
    auto arg1 = (::uint32)(::uint32_t)i;
    auto __ret = ((::GeometryBuffer*)NativePtr)->getAttribute(arg0, arg1);
    if (__ret == nullptr) return System::IntPtr();
    return IntPtr(__ret);
}

char Flood::GeometryBuffer::GetAttributeStride(Flood::VertexAttribute _0)
{
    auto arg0 = (::VertexAttribute)_0;
    auto __ret = ((::GeometryBuffer*)NativePtr)->getAttributeStride(arg0);
    return __ret;
}

void Flood::GeometryBuffer::ClearIndexes()
{
    ((::GeometryBuffer*)NativePtr)->clearIndexes();
}

bool Flood::GeometryBuffer::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<GeometryBuffer^>(object);

    if (!obj) return false;
    return __Instance == obj->__Instance;
}

int Flood::GeometryBuffer::GetHashCode()
{
    return (int)NativePtr;
}

System::IntPtr Flood::GeometryBuffer::__Instance::get()
{
    return System::IntPtr(NativePtr);
}

void Flood::GeometryBuffer::__Instance::set(System::IntPtr object)
{
    NativePtr = (::GeometryBuffer*)object.ToPointer();
}

Flood::BufferUsage Flood::GeometryBuffer::BufferUsage::get()
{
    auto __ret = ((::GeometryBuffer*)NativePtr)->getBufferUsage();
    return (Flood::BufferUsage)__ret;
}

void Flood::GeometryBuffer::BufferUsage::set(Flood::BufferUsage value)
{
    auto v = value;
    auto arg0 = (::BufferUsage)v;
    ((::GeometryBuffer*)NativePtr)->setBufferUsage(arg0);
}

Flood::BufferAccess Flood::GeometryBuffer::BufferAccess::get()
{
    auto __ret = ((::GeometryBuffer*)NativePtr)->getBufferAccess();
    return (Flood::BufferAccess)__ret;
}

void Flood::GeometryBuffer::BufferAccess::set(Flood::BufferAccess value)
{
    auto v = value;
    auto arg0 = (::BufferAccess)v;
    ((::GeometryBuffer*)NativePtr)->setBufferAccess(arg0);
}

unsigned int Flood::GeometryBuffer::NumVertices::get()
{
    auto __ret = ((::GeometryBuffer*)NativePtr)->getNumVertices();
    return __ret;
}

unsigned int Flood::GeometryBuffer::NumIndices::get()
{
    auto __ret = ((::GeometryBuffer*)NativePtr)->getNumIndices();
    return __ret;
}

Flood::BufferUsage Flood::GeometryBuffer::Usage::get()
{
    return (Flood::BufferUsage)((::GeometryBuffer*)NativePtr)->usage;
}

void Flood::GeometryBuffer::Usage::set(Flood::BufferUsage value)
{
    ((::GeometryBuffer*)NativePtr)->usage = (::BufferUsage)value;
}

Flood::BufferAccess Flood::GeometryBuffer::Access::get()
{
    return (Flood::BufferAccess)((::GeometryBuffer*)NativePtr)->access;
}

void Flood::GeometryBuffer::Access::set(Flood::BufferAccess value)
{
    ((::GeometryBuffer*)NativePtr)->access = (::BufferAccess)value;
}

System::Collections::Generic::List<unsigned char>^ Flood::GeometryBuffer::Data::get()
{
    auto _tmp__Data = gcnew System::Collections::Generic::List<unsigned char>();
    for(auto _element : ((::GeometryBuffer*)NativePtr)->data)
    {
        auto _marshalElement = _element;
        _tmp__Data->Add(_marshalElement);
    }
    return _tmp__Data;
}

void Flood::GeometryBuffer::Data::set(System::Collections::Generic::List<unsigned char>^ value)
{
    auto _tmpvalue = std::vector<::uint8>();
    for each(unsigned char _element in value)
    {
        auto _marshalElement = (::uint8)(::uint8_t)_element;
        _tmpvalue.push_back(_marshalElement);
    }
    ((::GeometryBuffer*)NativePtr)->data = _tmpvalue;
}

System::Collections::Generic::List<unsigned char>^ Flood::GeometryBuffer::IndexData::get()
{
    auto _tmp__IndexData = gcnew System::Collections::Generic::List<unsigned char>();
    for(auto _element : ((::GeometryBuffer*)NativePtr)->indexData)
    {
        auto _marshalElement = _element;
        _tmp__IndexData->Add(_marshalElement);
    }
    return _tmp__IndexData;
}

void Flood::GeometryBuffer::IndexData::set(System::Collections::Generic::List<unsigned char>^ value)
{
    auto _tmpvalue = std::vector<::uint8>();
    for each(unsigned char _element in value)
    {
        auto _marshalElement = (::uint8)(::uint8_t)_element;
        _tmpvalue.push_back(_marshalElement);
    }
    ((::GeometryBuffer*)NativePtr)->indexData = _tmpvalue;
}

unsigned char Flood::GeometryBuffer::IndexSize::get()
{
    return ((::GeometryBuffer*)NativePtr)->indexSize;
}

void Flood::GeometryBuffer::IndexSize::set(unsigned char value)
{
    ((::GeometryBuffer*)NativePtr)->indexSize = (::uint8)(::uint8_t)value;
}

bool Flood::GeometryBuffer::NeedsRebuild::get()
{
    return ((::GeometryBuffer*)NativePtr)->needsRebuild;
}

void Flood::GeometryBuffer::NeedsRebuild::set(bool value)
{
    ((::GeometryBuffer*)NativePtr)->needsRebuild = value;
}

unsigned int Flood::GeometryBuffer::Hash::get()
{
    return ((::GeometryBuffer*)NativePtr)->hash;
}

void Flood::GeometryBuffer::Hash::set(unsigned int value)
{
    ((::GeometryBuffer*)NativePtr)->hash = (::uint32)(::uint32_t)value;
}

Flood::VertexDeclaration^ Flood::GeometryBuffer::Declarations::get()
{
    return gcnew Flood::VertexDeclaration((::VertexDeclaration*)&((::GeometryBuffer*)NativePtr)->declarations);
}

void Flood::GeometryBuffer::Declarations::set(Flood::VertexDeclaration^ value)
{
    ((::GeometryBuffer*)NativePtr)->declarations = *(::VertexDeclaration*)value->NativePtr;
}

