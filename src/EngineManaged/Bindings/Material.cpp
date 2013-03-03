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
    : Resource(native)
{
}

Flood::Material::Material(System::IntPtr native)
    : Resource(native)
{
    auto __native = (::Material*)native.ToPointer();
}

Flood::Material::Material()
    : Resource(nullptr)
{
    NativePtr = new ::Material();
}

Flood::Material::Material(System::String^ name)
    : Resource(nullptr)
{
    auto arg0 = marshalString<E_UTF8>(name);
    NativePtr = new ::Material(arg0);
}

Flood::Class^ Flood::Material::GetType()
{
    auto ret = ((::Material*)NativePtr)->getType();
    return gcnew Flood::Class((::Class*)ret);
}

Flood::Class^ Flood::Material::GetStaticType()
{
    auto ret = ((::Material*)NativePtr)->getStaticType();
    return gcnew Flood::Class((::Class*)ret);
}

Flood::ResourceGroup Flood::Material::GetResourceGroup()
{
    auto ret = ((::Material*)NativePtr)->getResourceGroup();
    return (Flood::ResourceGroup)ret;
}

System::String^ Flood::Material::GetName()
{
    auto ret = &((::Material*)NativePtr)->getName();
    return marshalString<E_UTF8>(*ret);
}

void Flood::Material::SetName(System::String^ v)
{
    auto arg0 = marshalString<E_UTF8>(v);
    ((::Material*)NativePtr)->setName(arg0);
}

void Flood::Material::SetShader(System::String^ name)
{
    auto arg0 = marshalString<E_UTF8>(name);
    ((::Material*)NativePtr)->setShader(arg0);
}

bool Flood::Material::GetDepthWrite()
{
    auto ret = ((::Material*)NativePtr)->getDepthWrite();
    return ret;
}

void Flood::Material::SetDepthWrite(bool v)
{
    ((::Material*)NativePtr)->setDepthWrite(v);
}

bool Flood::Material::GetDepthTest()
{
    auto ret = ((::Material*)NativePtr)->getDepthTest();
    return ret;
}

void Flood::Material::SetDepthTest(bool v)
{
    ((::Material*)NativePtr)->setDepthTest(v);
}

Flood::DepthCompare Flood::Material::GetDepthCompare()
{
    auto ret = ((::Material*)NativePtr)->getDepthCompare();
    return (Flood::DepthCompare)ret;
}

void Flood::Material::SetDepthCompare(Flood::DepthCompare v)
{
    auto arg0 = (::DepthCompare)v;
    ((::Material*)NativePtr)->setDepthCompare(arg0);
}

Flood::Vector2 Flood::Material::GetDepthRange()
{
    auto ret = ((::Material*)NativePtr)->getDepthRange();
    return Flood::Vector2((::Vector2*)&ret);
}

void Flood::Material::SetDepthRange(Flood::Vector2 v)
{
    auto arg0 = ::Vector2();
    ((::Material*)NativePtr)->setDepthRange(arg0);
}

Flood::Vector2 Flood::Material::GetDepthOffset()
{
    auto ret = ((::Material*)NativePtr)->getDepthOffset();
    return Flood::Vector2((::Vector2*)&ret);
}

void Flood::Material::SetDepthOffset(Flood::Vector2 v)
{
    auto arg0 = ::Vector2();
    ((::Material*)NativePtr)->setDepthOffset(arg0);
}

bool Flood::Material::GetAlphaTest()
{
    auto ret = ((::Material*)NativePtr)->getAlphaTest();
    return ret;
}

void Flood::Material::SetAlphaTest(bool v)
{
    ((::Material*)NativePtr)->setAlphaTest(v);
}

float Flood::Material::GetLineWidth()
{
    auto ret = ((::Material*)NativePtr)->getLineWidth();
    return ret;
}

void Flood::Material::SetLineWidth(float v)
{
    ((::Material*)NativePtr)->setLineWidth(v);
}

bool Flood::Material::GetLineSmoothing()
{
    auto ret = ((::Material*)NativePtr)->getLineSmoothing();
    return ret;
}

void Flood::Material::SetLineSmoothing(bool v)
{
    ((::Material*)NativePtr)->setLineSmoothing(v);
}

bool Flood::Material::GetBackfaceCulling()
{
    auto ret = ((::Material*)NativePtr)->getBackfaceCulling();
    return ret;
}

void Flood::Material::SetBackfaceCulling(bool v)
{
    ((::Material*)NativePtr)->setBackfaceCulling(v);
}

Flood::BlendSource Flood::Material::GetBlendSource()
{
    auto ret = ((::Material*)NativePtr)->getBlendSource();
    return (Flood::BlendSource)ret;
}

Flood::BlendDestination Flood::Material::GetBlendDestination()
{
    auto ret = ((::Material*)NativePtr)->getBlendDestination();
    return (Flood::BlendDestination)ret;
}

bool Flood::Material::IsBlendingEnabled()
{
    auto ret = ((::Material*)NativePtr)->isBlendingEnabled();
    return ret;
}

void Flood::Material::SetBlending(Flood::BlendSource _215, Flood::BlendDestination _216)
{
    auto arg0 = (::BlendSource)_215;
    auto arg1 = (::BlendDestination)_216;
    ((::Material*)NativePtr)->setBlending(arg0, arg1);
}

void Flood::Material::SetTexture(unsigned char unit, System::String^ name)
{
    auto arg0 = (uint8)unit;
    auto arg1 = marshalString<E_UTF8>(name);
    ((::Material*)NativePtr)->setTexture(arg0, arg1);
}

Flood::TextureUnit^ Flood::Material::GetTextureUnit(unsigned char unit)
{
    auto arg0 = (uint8)unit;
    auto ret = &((::Material*)NativePtr)->getTextureUnit(arg0);
    return gcnew Flood::TextureUnit((::TextureUnit*)&ret);
}

void Flood::Material::Init()
{
    ((::Material*)NativePtr)->init();
}

