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
    __Attribute = (Flood::VertexAttribute)native->attribute;
    __Type = (Flood::VertexDataType)native->type;
    __Components = native->components;
}

Flood::VertexElementP::VertexElementP(System::IntPtr native)
{
    auto __native = (::VertexElementP*)native.ToPointer();
    __Attribute = (Flood::VertexAttribute)__native->attribute;
    __Type = (Flood::VertexDataType)__native->type;
    __Components = __native->components;
}

Flood::VertexAttribute Flood::VertexElementP::Attribute::get()
{
    return __Attribute;
}

void Flood::VertexElementP::Attribute::set(Flood::VertexAttribute value)
{
    __Attribute = value;
}

Flood::VertexDataType Flood::VertexElementP::Type::get()
{
    return __Type;
}

void Flood::VertexElementP::Type::set(Flood::VertexDataType value)
{
    __Type = value;
}

unsigned char Flood::VertexElementP::Components::get()
{
    return __Components;
}

void Flood::VertexElementP::Components::set(unsigned char value)
{
    __Components = value;
}

Flood::VertexElement::VertexElement(::VertexElement* native)
{
    __Attribute = (Flood::VertexAttribute)native->attribute;
    __Type = (Flood::VertexDataType)native->type;
    __Components = native->components;
    __Stride = native->stride;
    __Offset = native->offset;
    __Size = native->size;
}

Flood::VertexElement::VertexElement(System::IntPtr native)
{
    auto __native = (::VertexElement*)native.ToPointer();
    __Attribute = (Flood::VertexAttribute)__native->attribute;
    __Type = (Flood::VertexDataType)__native->type;
    __Components = __native->components;
    __Stride = __native->stride;
    __Offset = __native->offset;
    __Size = __native->size;
}

Flood::VertexElement::VertexElement(Flood::VertexAttribute _0, Flood::VertexDataType _1, unsigned char components)
{
    ::VertexElement _native((::VertexAttribute)_0, (::VertexDataType)_1, (::uint8)(::uint8_t)components);
    this->Attribute = (Flood::VertexAttribute)_native.attribute;
    this->Type = (Flood::VertexDataType)_native.type;
    this->Components = _native.components;
    this->Stride = _native.stride;
    this->Offset = _native.offset;
    this->Size1 = _native.size;
}

Flood::VertexAttribute Flood::VertexElement::Attribute::get()
{
    return __Attribute;
}

void Flood::VertexElement::Attribute::set(Flood::VertexAttribute value)
{
    __Attribute = value;
}

Flood::VertexDataType Flood::VertexElement::Type::get()
{
    return __Type;
}

void Flood::VertexElement::Type::set(Flood::VertexDataType value)
{
    __Type = value;
}

unsigned char Flood::VertexElement::Components::get()
{
    return __Components;
}

void Flood::VertexElement::Components::set(unsigned char value)
{
    __Components = value;
}

unsigned char Flood::VertexElement::Size::get()
{
    auto _this0 = ::VertexElement();
    _this0.attribute = (::VertexAttribute)(*this).Attribute;
    _this0.type = (::VertexDataType)(*this).Type;
    _this0.components = (::uint8)(::uint8_t)(*this).Components;
    _this0.stride = (::int8)(::int8_t)(*this).Stride;
    _this0.offset = (::uint32)(::uint32_t)(*this).Offset;
    _this0.size = (::uint32)(::uint32_t)(*this).Size1;
    auto __ret = _this0.getSize();
    __Attribute = (Flood::VertexAttribute)_this0.attribute;
    __Type = (Flood::VertexDataType)_this0.type;
    __Components = _this0.components;
    __Stride = _this0.stride;
    __Offset = _this0.offset;
    __Size = _this0.size;
    return __ret;
}

char Flood::VertexElement::Stride::get()
{
    return __Stride;
}

void Flood::VertexElement::Stride::set(char value)
{
    __Stride = value;
}

unsigned int Flood::VertexElement::Offset::get()
{
    return __Offset;
}

void Flood::VertexElement::Offset::set(unsigned int value)
{
    __Offset = value;
}

unsigned int Flood::VertexElement::Size1::get()
{
    return __Size;
}

void Flood::VertexElement::Size1::set(unsigned int value)
{
    __Size = value;
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
    _marshal0.components = (::uint8)(::uint8_t)_0.Components;
    _marshal0.stride = (::int8)(::int8_t)_0.Stride;
    _marshal0.offset = (::uint32)(::uint32_t)_0.Offset;
    _marshal0.size = (::uint32)(::uint32_t)_0.Size1;
    auto arg0 = _marshal0;
    ((::VertexDeclaration*)NativePtr)->add(arg0);
}

void Flood::VertexDeclaration::Add(Flood::VertexElementP _0)
{
    auto _marshal0 = ::VertexElementP();
    _marshal0.attribute = (::VertexAttribute)_0.Attribute;
    _marshal0.type = (::VertexDataType)_0.Type;
    _marshal0.components = (::uint8)(::uint8_t)_0.Components;
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
    auto __ret = ((::VertexDeclaration*)NativePtr)->find(arg0);
    return Flood::VertexElement((::VertexElement*)__ret);
}

unsigned char Flood::VertexDeclaration::GetOffset(Flood::VertexAttribute _0)
{
    auto arg0 = (::VertexAttribute)_0;
    auto __ret = ((::VertexDeclaration*)NativePtr)->getOffset(arg0);
    return __ret;
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
    return __Instance == obj->__Instance;
}

int Flood::VertexDeclaration::GetHashCode()
{
    return (int)NativePtr;
}

System::IntPtr Flood::VertexDeclaration::__Instance::get()
{
    return System::IntPtr(NativePtr);
}

void Flood::VertexDeclaration::__Instance::set(System::IntPtr object)
{
    NativePtr = (::VertexDeclaration*)object.ToPointer();
}

unsigned char Flood::VertexDeclaration::VertexSize::get()
{
    auto __ret = ((::VertexDeclaration*)NativePtr)->getVertexSize();
    return __ret;
}

System::Collections::Generic::List<Flood::VertexElement>^ Flood::VertexDeclaration::Decls::get()
{
    auto _tmp__Decls = gcnew System::Collections::Generic::List<Flood::VertexElement>();
    for(auto _element : ((::VertexDeclaration*)NativePtr)->decls)
    {
        auto _marshalElement = Flood::VertexElement((::VertexElement*)&_element);
        _tmp__Decls->Add(_marshalElement);
    }
    return _tmp__Decls;
}

void Flood::VertexDeclaration::Decls::set(System::Collections::Generic::List<Flood::VertexElement>^ value)
{
    auto _tmpvalue = std::vector<::VertexElement>();
    for each(Flood::VertexElement _element in value)
    {
        auto _marshal0 = ::VertexElement();
        _marshal0.attribute = (::VertexAttribute)_element.Attribute;
        _marshal0.type = (::VertexDataType)_element.Type;
        _marshal0.components = (::uint8)(::uint8_t)_element.Components;
        _marshal0.stride = (::int8)(::int8_t)_element.Stride;
        _marshal0.offset = (::uint32)(::uint32_t)_element.Offset;
        _marshal0.size = (::uint32)(::uint32_t)_element.Size1;
        auto _marshalElement = _marshal0;
        _tmpvalue.push_back(_marshalElement);
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
    auto __ret = ((::VertexBuffer*)NativePtr)->isBuilt();
    return __ret;
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
    return __Instance == obj->__Instance;
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

