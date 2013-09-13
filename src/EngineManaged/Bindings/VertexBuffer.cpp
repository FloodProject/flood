/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "VertexBuffer.h"

using namespace System;
using namespace System::Runtime::InteropServices;

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
    Attribute = (Flood::VertexAttribute)native->attribute;
    Type = (Flood::VertexDataType)native->type;
    Components = native->components;
    Stride = native->stride;
    Offset = native->offset;
    Size = native->size;
}

Flood::VertexElement::VertexElement(System::IntPtr native)
{
    auto __native = (::VertexElement*)native.ToPointer();
    Attribute = (Flood::VertexAttribute)__native->attribute;
    Type = (Flood::VertexDataType)__native->type;
    Components = __native->components;
    Stride = __native->stride;
    Offset = __native->offset;
    Size = __native->size;
}

Flood::VertexElement::VertexElement(Flood::VertexAttribute _0, Flood::VertexDataType _1, unsigned char components)
{
    ::VertexElement _native((::VertexAttribute)_0, (::VertexDataType)_1, (::uint8)components);
    this->Attribute = (Flood::VertexAttribute)_native.attribute;
    this->Type = (Flood::VertexDataType)_native.type;
    this->Components = _native.components;
    this->Stride = _native.stride;
    this->Offset = _native.offset;
    this->Size = _native.size;
}

unsigned char Flood::VertexElement::Size1::get()
{
    auto _this0 = ::VertexElement();
    _this0.attribute = (::VertexAttribute)(*this).Attribute;
    _this0.type = (::VertexDataType)(*this).Type;
    _this0.components = (::uint8)(*this).Components;
    _this0.stride = (::int8)(*this).Stride;
    _this0.offset = (::uint32)(*this).Offset;
    _this0.size = (::uint32)(*this).Size;
    auto ret = _this0.getSize();
    Attribute = (Flood::VertexAttribute)_this0.attribute;
    Type = (Flood::VertexDataType)_this0.type;
    Components = _this0.components;
    Stride = _this0.stride;
    Offset = _this0.offset;
    Size = _this0.size;
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

void Flood::VertexDeclaration::Add(Flood::VertexAttribute _0, int numComponents)
{
    auto arg0 = (::VertexAttribute)_0;
    ((::VertexDeclaration*)NativePtr)->add(arg0, numComponents);
}

void Flood::VertexDeclaration::Add(Flood::VertexElement _0)
{
    auto _marshal0 = ::VertexElement();
    _marshal0.attribute = (::VertexAttribute)_0.Attribute;
    _marshal0.type = (::VertexDataType)_0.Type;
    _marshal0.components = (::uint8)_0.Components;
    _marshal0.stride = (::int8)_0.Stride;
    _marshal0.offset = (::uint32)_0.Offset;
    _marshal0.size = (::uint32)_0.Size;
    auto arg0 = _marshal0;
    ((::VertexDeclaration*)NativePtr)->add(arg0);
}

void Flood::VertexDeclaration::Add(Flood::VertexElementP _0)
{
    auto _marshal0 = ::VertexElementP();
    _marshal0.attribute = (::VertexAttribute)_0.Attribute;
    _marshal0.type = (::VertexDataType)_0.Type;
    _marshal0.components = (::uint8)_0.Components;
    auto arg0 = _marshal0;
    ((::VertexDeclaration*)NativePtr)->add(arg0);
}

void Flood::VertexDeclaration::Reset()
{
    ((::VertexDeclaration*)NativePtr)->reset();
}

Flood::VertexElement Flood::VertexDeclaration::Find(Flood::VertexAttribute _0)
{
    auto arg0 = (::VertexAttribute)_0;
    auto ret = ((::VertexDeclaration*)NativePtr)->find(arg0);
    return Flood::VertexElement((::VertexElement*)ret);
}

unsigned char Flood::VertexDeclaration::GetOffset(Flood::VertexAttribute _0)
{
    auto arg0 = (::VertexAttribute)_0;
    auto ret = ((::VertexDeclaration*)NativePtr)->getOffset(arg0);
    return ret;
}

void Flood::VertexDeclaration::CalculateStrides()
{
    ((::VertexDeclaration*)NativePtr)->calculateStrides();
}

Flood::VertexDeclaration::VertexDeclaration()
{
    NativePtr = new ::VertexDeclaration();
}

bool Flood::VertexDeclaration::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<VertexDeclaration^>(object);

    if (!obj) return false;
    return Instance == obj->Instance;
}

int Flood::VertexDeclaration::GetHashCode()
{
    return (int)NativePtr;
}

System::IntPtr Flood::VertexDeclaration::Instance::get()
{
    return System::IntPtr(NativePtr);
}

void Flood::VertexDeclaration::Instance::set(System::IntPtr object)
{
    NativePtr = (::VertexDeclaration*)object.ToPointer();
}

unsigned char Flood::VertexDeclaration::VertexSize::get()
{
    auto ret = ((::VertexDeclaration*)NativePtr)->getVertexSize();
    return ret;
}

System::Collections::Generic::List<Flood::VertexElement>^ Flood::VertexDeclaration::Decls::get()
{
    auto _tmpDecls = gcnew System::Collections::Generic::List<Flood::VertexElement>();
    for(auto _element : ((::VertexDeclaration*)NativePtr)->decls)
    {
        auto _marshalElement = Flood::VertexElement((::VertexElement*)&_element);
        _tmpDecls->Add(_marshalElement);
    }
    return _tmpDecls;
}

void Flood::VertexDeclaration::Decls::set(System::Collections::Generic::List<Flood::VertexElement>^ value)
{
    auto _tmpvalue = std::vector<::VertexElement>();
    for each(Flood::VertexElement _element in value)
    {
        auto _marshal0 = ::VertexElement();
        _marshal0.attribute = (::VertexAttribute)_element.Attribute;
        _marshal0.type = (::VertexDataType)_element.Type;
        _marshal0.components = (::uint8)_element.Components;
        _marshal0.stride = (::int8)_element.Stride;
        _marshal0.offset = (::uint32)_element.Offset;
        _marshal0.size = (::uint32)_element.Size;
        auto _marshalElement = _marshal0;
        _tmpvalue.pushBack(_marshalElement);
    }
    ((::VertexDeclaration*)NativePtr)->decls = _tmpvalue;
}

Flood::VertexBuffer::VertexBuffer(::VertexBuffer* native)
    : Flood::Buffer((::Buffer*)native)
{
}

Flood::VertexBuffer::VertexBuffer(System::IntPtr native)
    : Flood::Buffer(native)
{
    auto __native = (::VertexBuffer*)native.ToPointer();
}

Flood::VertexBuffer::VertexBuffer()
    : Flood::Buffer((::Buffer*)nullptr)
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

bool Flood::VertexBuffer::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<VertexBuffer^>(object);

    if (!obj) return false;
    return Instance == obj->Instance;
}

int Flood::VertexBuffer::GetHashCode()
{
    return (int)NativePtr;
}

bool Flood::VertexBuffer::Built::get()
{
    return ((::VertexBuffer*)NativePtr)->built;
}

void Flood::VertexBuffer::Built::set(bool value)
{
    ((::VertexBuffer*)NativePtr)->built = value;
}

