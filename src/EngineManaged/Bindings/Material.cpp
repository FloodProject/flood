/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "Material.h"
#include "Image.h"
#include "Memory.h"
#include "Resource.h"
#include "ResourceHandle.h"
#include "ShaderMaterial.h"
#include "Texture.h"
#include "Vector.h"

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

Flood::ResourceGroup Flood::Material::GetResourceGroup()
{
    auto ret = ((::Material*)NativePtr)->getResourceGroup();
    return (Flood::ResourceGroup)ret;
}

System::String^ Flood::Material::GetName()
{
    auto ret = ((::Material*)NativePtr)->getName();
    return marshalString<E_UTF8>(ret);
}

void Flood::Material::SetName(System::String^ v)
{
    auto arg0 = marshalString<E_UTF8>(v);
    ((::Material*)NativePtr)->setName(arg0);
}

Flood::ResourceHandle<Flood::ShaderMaterial^> Flood::Material::GetShader()
{
    auto ret = ((::Material*)NativePtr)->getShader();
    return Flood::ResourceHandle<Flood::ShaderMaterial^>(ret.id);
}

void Flood::Material::SetShader(Flood::ResourceHandle<Flood::ShaderMaterial^> v)
{
    auto arg0 = (HandleId)v.Id;
    ((::Material*)NativePtr)->setShader(arg0);
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

void Flood::Material::SetBlending(Flood::BlendSource _203, Flood::BlendDestination _204)
{
    auto arg0 = (::BlendSource)_203;
    auto arg1 = (::BlendDestination)_204;
    ((::Material*)NativePtr)->setBlending(arg0, arg1);
}

void Flood::Material::SetTexture(unsigned char unit, System::String^ name)
{
    auto arg0 = (uint8)unit;
    auto arg1 = marshalString<E_UTF8>(name);
    ((::Material*)NativePtr)->setTexture(arg0, arg1);
}

void Flood::Material::SetTexture(unsigned char unit, Flood::ResourceHandle<Flood::Image^> image)
{
    auto arg0 = (uint8)unit;
    auto arg1 = (HandleId)image.Id;
    ((::Material*)NativePtr)->setTexture(arg0, arg1);
}

Flood::ResourceHandle<Flood::Image^> Flood::Material::GetTexture(unsigned char unit)
{
    auto arg0 = (uint8)unit;
    auto ret = ((::Material*)NativePtr)->getTexture(arg0);
    return Flood::ResourceHandle<Flood::Image^>(ret.id);
}

Flood::TextureUnit^ Flood::Material::GetTextureUnit(unsigned char unit)
{
    auto arg0 = (uint8)unit;
    auto ret = ((::Material*)NativePtr)->getTextureUnit(arg0);
    return gcnew Flood::TextureUnit((::TextureUnit*)&ret);
}

void Flood::Material::Init()
{
    ((::Material*)NativePtr)->init();
}

Flood::ResourceHandle<Flood::Material^> Flood::Material::Create(Flood::Allocator^ _202, System::String^ name)
{
    auto arg0 = (::Allocator*)_202->NativePtr;
    auto arg1 = marshalString<E_UTF8>(name);
    auto ret = ::MaterialCreate(arg0, arg1);
    return Flood::ResourceHandle<Flood::Material^>(ret.id);
}

