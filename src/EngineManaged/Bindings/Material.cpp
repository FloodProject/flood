/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "Material.h"
#include "Reflection.h"
#include "Memory.h"
#include "Serialization.h"
#include "Resource.h"
#include "Vector.h"
#include "Texture.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace clix;

Flood::Material::Material(::Material* native)
{
    NativePtr = native;
}

Flood::Material::Material(System::IntPtr native)
{
    NativePtr = (::Material*)native.ToPointer();
}

Flood::Material::Material()
{
    NativePtr = new ::Material();
}

Flood::Material::Material(System::String^ name)
{
    auto arg0 = marshalString<E_UTF8>(name);
    NativePtr = new ::Material(arg0);
}

Flood::Class^ Flood::Material::GetType()
{
    auto ret = NativePtr->getType();
    return gcnew Flood::Class((::Class*)ret);
}

Flood::Class^ Flood::Material::GetStaticType()
{
    auto ret = NativePtr->getStaticType();
    return gcnew Flood::Class((::Class*)ret);
}

Flood::ResourceGroup Flood::Material::GetResourceGroup()
{
    auto ret = NativePtr->getResourceGroup();
    return (Flood::ResourceGroup)ret;
}

System::String^ Flood::Material::GetName()
{
    auto ret = &NativePtr->getName();
    return marshalString<E_UTF8>(*ret);
}

void Flood::Material::SetName(System::String^ v)
{
    auto arg0 = marshalString<E_UTF8>(v);
    NativePtr->setName(arg0);
}

void Flood::Material::SetShader(System::String^ name)
{
    auto arg0 = marshalString<E_UTF8>(name);
    NativePtr->setShader(arg0);
}

bool Flood::Material::GetDepthWrite()
{
    auto ret = NativePtr->getDepthWrite();
    return ret;
}

void Flood::Material::SetDepthWrite(bool v)
{
    NativePtr->setDepthWrite(v);
}

bool Flood::Material::GetDepthTest()
{
    auto ret = NativePtr->getDepthTest();
    return ret;
}

void Flood::Material::SetDepthTest(bool v)
{
    NativePtr->setDepthTest(v);
}

Flood::DepthCompare Flood::Material::GetDepthCompare()
{
    auto ret = NativePtr->getDepthCompare();
    return (Flood::DepthCompare)ret;
}

void Flood::Material::SetDepthCompare(Flood::DepthCompare v)
{
    auto arg0 = (::DepthCompare)v;
    NativePtr->setDepthCompare(arg0);
}

Flood::Vector2 Flood::Material::GetDepthRange()
{
    auto ret = NativePtr->getDepthRange();
    return Flood::Vector2((::Vector2*)&ret);
}

void Flood::Material::SetDepthRange(Flood::Vector2 v)
{
    auto arg0 = ::Vector2();
    NativePtr->setDepthRange(arg0);
}

Flood::Vector2 Flood::Material::GetDepthOffset()
{
    auto ret = NativePtr->getDepthOffset();
    return Flood::Vector2((::Vector2*)&ret);
}

void Flood::Material::SetDepthOffset(Flood::Vector2 v)
{
    auto arg0 = ::Vector2();
    NativePtr->setDepthOffset(arg0);
}

bool Flood::Material::GetAlphaTest()
{
    auto ret = NativePtr->getAlphaTest();
    return ret;
}

void Flood::Material::SetAlphaTest(bool v)
{
    NativePtr->setAlphaTest(v);
}

float Flood::Material::GetLineWidth()
{
    auto ret = NativePtr->getLineWidth();
    return ret;
}

void Flood::Material::SetLineWidth(float v)
{
    NativePtr->setLineWidth(v);
}

bool Flood::Material::GetLineSmoothing()
{
    auto ret = NativePtr->getLineSmoothing();
    return ret;
}

void Flood::Material::SetLineSmoothing(bool v)
{
    NativePtr->setLineSmoothing(v);
}

bool Flood::Material::GetBackfaceCulling()
{
    auto ret = NativePtr->getBackfaceCulling();
    return ret;
}

void Flood::Material::SetBackfaceCulling(bool v)
{
    NativePtr->setBackfaceCulling(v);
}

Flood::BlendSource Flood::Material::GetBlendSource()
{
    auto ret = NativePtr->getBlendSource();
    return (Flood::BlendSource)ret;
}

Flood::BlendDestination Flood::Material::GetBlendDestination()
{
    auto ret = NativePtr->getBlendDestination();
    return (Flood::BlendDestination)ret;
}

bool Flood::Material::IsBlendingEnabled()
{
    auto ret = NativePtr->isBlendingEnabled();
    return ret;
}

void Flood::Material::SetBlending(Flood::BlendSource _214, Flood::BlendDestination _215)
{
    auto arg0 = (::BlendSource)_214;
    auto arg1 = (::BlendDestination)_215;
    NativePtr->setBlending(arg0, arg1);
}

void Flood::Material::SetTexture(unsigned char unit, System::String^ name)
{
    auto arg0 = (uint8)unit;
    auto arg1 = marshalString<E_UTF8>(name);
    NativePtr->setTexture(arg0, arg1);
}

Flood::TextureUnit^ Flood::Material::GetTextureUnit(unsigned char unit)
{
    auto arg0 = (uint8)unit;
    auto ret = &NativePtr->getTextureUnit(arg0);
    return gcnew Flood::TextureUnit((::TextureUnit*)ret);
}

void Flood::Material::Init()
{
    NativePtr->init();
}

