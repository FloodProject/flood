/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Material.h"
#include "Memory.h"
#include "Resource.h"
#include "Texture.h"

using namespace System;
using namespace System::Runtime::InteropServices;

Flood::Material::Material(::Material* native)
    : Flood::Resource((::Resource*)native)
{
}

Flood::Material::Material(System::IntPtr native)
    : Flood::Resource(native)
{
    auto __native = (::Material*)native.ToPointer();
}

Flood::Material::Material()
    : Flood::Resource((::Resource*)nullptr)
{
    NativePtr = new ::Material();
}

Flood::Material::Material(System::String^ name)
    : Flood::Resource((::Resource*)nullptr)
{
    auto arg0 = StringMarshaller::marshalString(name);
    NativePtr = new ::Material(arg0);
}

void Flood::Material::SetShader(System::String^ name)
{
    auto arg0 = StringMarshaller::marshalString(name);
    ((::Material*)NativePtr)->setShader(arg0);
}

bool Flood::Material::IsBlendingEnabled()
{
    auto __ret = ((::Material*)NativePtr)->isBlendingEnabled();
    return __ret;
}

void Flood::Material::SetBlending(Flood::BlendSource _0, Flood::BlendDestination _1)
{
    auto arg0 = (::BlendSource)_0;
    auto arg1 = (::BlendDestination)_1;
    ((::Material*)NativePtr)->setBlending(arg0, arg1);
}

void Flood::Material::SetTexture(unsigned char unit, System::String^ name)
{
    auto arg0 = (::uint8)(::uint8_t)unit;
    auto arg1 = StringMarshaller::marshalString(name);
    ((::Material*)NativePtr)->setTexture(arg0, arg1);
}

void Flood::Material::SetTexture(unsigned char unit, Flood::ResourceHandle<Flood::Image^> image)
{
    auto arg0 = (::uint8)(::uint8_t)unit;
    auto arg1 = (HandleId)image.Id;
    ((::Material*)NativePtr)->setTexture(arg0, arg1);
}

Flood::ResourceHandle<Flood::Image^> Flood::Material::GetTexture(unsigned char unit)
{
    auto arg0 = (::uint8)(::uint8_t)unit;
    auto __ret = ((::Material*)NativePtr)->getTexture(arg0);
    return Flood::ResourceHandle<Flood::Image^>(__ret.id);
}

Flood::TextureUnit^ Flood::Material::GetTextureUnit(unsigned char unit)
{
    auto arg0 = (::uint8)(::uint8_t)unit;
    auto &__ret = ((::Material*)NativePtr)->getTextureUnit(arg0);
    return gcnew Flood::TextureUnit((::TextureUnit*)&__ret);
}

void Flood::Material::Init()
{
    ((::Material*)NativePtr)->init();
}

bool Flood::Material::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<Material^>(object);

    if (!obj) return false;
    return Instance == obj->Instance;
}

int Flood::Material::GetHashCode()
{
    return (int)NativePtr;
}

Flood::ResourceHandle<Flood::Material^> Flood::Material::Create(Flood::Allocator^ _0, System::String^ name)
{
    auto arg0 = (::Allocator*)_0->NativePtr;
    auto arg1 = StringMarshaller::marshalString(name);
    auto __ret = ::MaterialCreate(arg0, arg1);
    return Flood::ResourceHandle<Flood::Material^>(__ret.id);
}

Flood::ResourceGroup Flood::Material::ResourceGroup::get()
{
    auto __ret = ((::Material*)NativePtr)->getResourceGroup();
    return (Flood::ResourceGroup)__ret;
}

System::String^ Flood::Material::Name::get()
{
    auto &__ret = ((::Material*)NativePtr)->getName();
    return StringMarshaller::marshalString(__ret);
}

void Flood::Material::Name::set(System::String^ value)
{
    auto v = value;
    auto arg0 = StringMarshaller::marshalString(v);
    ((::Material*)NativePtr)->setName(arg0);
}

Flood::ResourceHandle<Flood::ShaderMaterial^> Flood::Material::Shader::get()
{
    auto &__ret = ((::Material*)NativePtr)->getShader();
    return Flood::ResourceHandle<Flood::ShaderMaterial^>(__ret.id);
}

void Flood::Material::Shader::set(Flood::ResourceHandle<Flood::ShaderMaterial^> value)
{
    auto v = value;
    auto arg0 = (HandleId)v.Id;
    ((::Material*)NativePtr)->setShader(arg0);
}

bool Flood::Material::DepthWrite::get()
{
    auto __ret = ((::Material*)NativePtr)->getDepthWrite();
    return __ret;
}

void Flood::Material::DepthWrite::set(bool value)
{
    auto v = value;
    ((::Material*)NativePtr)->setDepthWrite(v);
}

bool Flood::Material::DepthTest::get()
{
    auto __ret = ((::Material*)NativePtr)->getDepthTest();
    return __ret;
}

void Flood::Material::DepthTest::set(bool value)
{
    auto v = value;
    ((::Material*)NativePtr)->setDepthTest(v);
}

Flood::DepthCompare Flood::Material::DepthCompare::get()
{
    auto __ret = ((::Material*)NativePtr)->getDepthCompare();
    return (Flood::DepthCompare)__ret;
}

void Flood::Material::DepthCompare::set(Flood::DepthCompare value)
{
    auto v = value;
    auto arg0 = (::DepthCompare)v;
    ((::Material*)NativePtr)->setDepthCompare(arg0);
}

Flood::Vector2 Flood::Material::DepthRange::get()
{
    auto __ret = ((::Material*)NativePtr)->getDepthRange();
    return Flood::Vector2((::Vector2*)&__ret);
}

void Flood::Material::DepthRange::set(Flood::Vector2 value)
{
    auto v = value;
    auto _marshal0 = ::Vector2();
    _marshal0.x = v.X;
    _marshal0.y = v.Y;
    auto arg0 = _marshal0;
    ((::Material*)NativePtr)->setDepthRange(arg0);
}

Flood::Vector2 Flood::Material::DepthOffset::get()
{
    auto __ret = ((::Material*)NativePtr)->getDepthOffset();
    return Flood::Vector2((::Vector2*)&__ret);
}

void Flood::Material::DepthOffset::set(Flood::Vector2 value)
{
    auto v = value;
    auto _marshal0 = ::Vector2();
    _marshal0.x = v.X;
    _marshal0.y = v.Y;
    auto arg0 = _marshal0;
    ((::Material*)NativePtr)->setDepthOffset(arg0);
}

bool Flood::Material::AlphaTest::get()
{
    auto __ret = ((::Material*)NativePtr)->getAlphaTest();
    return __ret;
}

void Flood::Material::AlphaTest::set(bool value)
{
    auto v = value;
    ((::Material*)NativePtr)->setAlphaTest(v);
}

float Flood::Material::LineWidth::get()
{
    auto __ret = ((::Material*)NativePtr)->getLineWidth();
    return __ret;
}

void Flood::Material::LineWidth::set(float value)
{
    auto v = value;
    ((::Material*)NativePtr)->setLineWidth(v);
}

bool Flood::Material::LineSmoothing::get()
{
    auto __ret = ((::Material*)NativePtr)->getLineSmoothing();
    return __ret;
}

void Flood::Material::LineSmoothing::set(bool value)
{
    auto v = value;
    ((::Material*)NativePtr)->setLineSmoothing(v);
}

bool Flood::Material::BackfaceCulling::get()
{
    auto __ret = ((::Material*)NativePtr)->getBackfaceCulling();
    return __ret;
}

void Flood::Material::BackfaceCulling::set(bool value)
{
    auto v = value;
    ((::Material*)NativePtr)->setBackfaceCulling(v);
}

Flood::BlendSource Flood::Material::BlendSource::get()
{
    auto __ret = ((::Material*)NativePtr)->getBlendSource();
    return (Flood::BlendSource)__ret;
}

Flood::BlendDestination Flood::Material::BlendDestination::get()
{
    auto __ret = ((::Material*)NativePtr)->getBlendDestination();
    return (Flood::BlendDestination)__ret;
}

System::Collections::Generic::Dictionary<unsigned char, Flood::TextureUnit^>^ Flood::Material::TextureUnits::get()
{
    auto _tmpTextureUnits = gcnew System::Collections::Generic::Dictionary<unsigned char, Flood::TextureUnit^>();
    for(auto _it = ((::Material*)NativePtr)->textureUnits.Begin(); _it != ((::Material*)NativePtr)->textureUnits.End(); ++_it)
    {
        auto& _key = _it->first;
        auto& _val = _it->second;
        auto _marshalKey = _key;
        auto ___val = new ::TextureUnit(_val);
        auto _marshalValue = gcnew Flood::TextureUnit((::TextureUnit*)___val);
        _tmpTextureUnits->Add(_marshalKey, _marshalValue);
    }
    return _tmpTextureUnits;
}

void Flood::Material::TextureUnits::set(System::Collections::Generic::Dictionary<unsigned char, Flood::TextureUnit^>^ value)
{
    auto _tmpvalue = HashMap<::uint8, ::TextureUnit>();
    auto _keys = value->Keys->GetEnumerator();
    while (_keys.MoveNext())
    {
        unsigned char _key = _keys.Current;
        Flood::TextureUnit^ _val = value[_key];
        auto _marshalKey = (::uint8)(::uint8_t)_key;
        auto _marshalValue = *(::TextureUnit*)_val->NativePtr;
        _tmpvalue[_marshalKey] = _marshalValue;
    }
    ((::Material*)NativePtr)->textureUnits = _tmpvalue;
}

System::String^ Flood::Material::Name1::get()
{
    return StringMarshaller::marshalString(((::Material*)NativePtr)->name);
}

void Flood::Material::Name1::set(System::String^ value)
{
    ((::Material*)NativePtr)->name = StringMarshaller::marshalString(value);
}

Flood::ResourceHandle<Flood::ShaderMaterial^> Flood::Material::Shader1::get()
{
    return Flood::ResourceHandle<Flood::ShaderMaterial^>(((::Material*)NativePtr)->shader.id);
}

void Flood::Material::Shader1::set(Flood::ResourceHandle<Flood::ShaderMaterial^> value)
{
    ((::Material*)NativePtr)->shader = (HandleId)value.Id;
}

bool Flood::Material::CullBackfaces::get()
{
    return ((::Material*)NativePtr)->cullBackfaces;
}

void Flood::Material::CullBackfaces::set(bool value)
{
    ((::Material*)NativePtr)->cullBackfaces = value;
}

bool Flood::Material::LineSmooth::get()
{
    return ((::Material*)NativePtr)->lineSmooth;
}

void Flood::Material::LineSmooth::set(bool value)
{
    ((::Material*)NativePtr)->lineSmooth = value;
}

Flood::BlendSource Flood::Material::Source::get()
{
    return (Flood::BlendSource)((::Material*)NativePtr)->source;
}

void Flood::Material::Source::set(Flood::BlendSource value)
{
    ((::Material*)NativePtr)->source = (::BlendSource)value;
}

Flood::BlendDestination Flood::Material::Destination::get()
{
    return (Flood::BlendDestination)((::Material*)NativePtr)->destination;
}

void Flood::Material::Destination::set(Flood::BlendDestination value)
{
    ((::Material*)NativePtr)->destination = (::BlendDestination)value;
}

bool Flood::Material::_isBlendingEnabled::get()
{
    return ((::Material*)NativePtr)->_isBlendingEnabled;
}

void Flood::Material::_isBlendingEnabled::set(bool value)
{
    ((::Material*)NativePtr)->_isBlendingEnabled = value;
}

float Flood::Material::DefaultLineWidth::get()
{
    return ::Material::DefaultLineWidth;
}

void Flood::Material::DefaultLineWidth::set(float value)
{
    ::Material::DefaultLineWidth = value;
}

