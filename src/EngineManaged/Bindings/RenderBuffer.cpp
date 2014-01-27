/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "RenderBuffer.h"
#include "Image.h"
#include "Texture.h"

using namespace System;
using namespace System::Runtime::InteropServices;

Flood::RenderBuffer::RenderBuffer(::RenderBuffer* native)
    : Flood::RenderTarget((::RenderTarget*)native)
{
}

Flood::RenderBuffer::RenderBuffer(System::IntPtr native)
    : Flood::RenderTarget(native)
{
    auto __native = (::RenderBuffer*)native.ToPointer();
}

void Flood::RenderBuffer::MakeCurrent()
{
    ((::RenderBuffer*)NativePtr)->makeCurrent();
}

void Flood::RenderBuffer::Bind()
{
    ((::RenderBuffer*)NativePtr)->bind();
}

void Flood::RenderBuffer::Unbind()
{
    ((::RenderBuffer*)NativePtr)->unbind();
}

bool Flood::RenderBuffer::Check()
{
    auto __ret = ((::RenderBuffer*)NativePtr)->check();
    return __ret;
}

void Flood::RenderBuffer::Update()
{
    ((::RenderBuffer*)NativePtr)->update();
}

void Flood::RenderBuffer::Read(char attachment, System::Collections::Generic::List<unsigned char>^ data)
{
    auto arg0 = (::int8)(::int8_t)attachment;
    auto _tmpdata = Vector<::uint8>();
    for each(unsigned char _element in data)
    {
        auto _marshalElement = (::uint8)(::uint8_t)_element;
        _tmpdata.Push(_marshalElement);
    }
    auto arg1 = _tmpdata;
    ((::RenderBuffer*)NativePtr)->read(arg0, arg1);
}

Flood::Image^ Flood::RenderBuffer::ReadImage(char attachment)
{
    auto arg0 = (::int8)(::int8_t)attachment;
    auto __ret = ((::RenderBuffer*)NativePtr)->readImage(arg0);
    if (__ret == nullptr) return nullptr;
    return gcnew Flood::Image((::Image*)__ret);
}

void Flood::RenderBuffer::CreateRenderBuffer(Flood::RenderBufferType _0)
{
    auto arg0 = (::RenderBufferType)_0;
    ((::RenderBuffer*)NativePtr)->createRenderBuffer(arg0);
}

Flood::Texture^ Flood::RenderBuffer::CreateRenderTexture(Flood::RenderBufferType _1)
{
    auto arg0 = (::RenderBufferType)_1;
    auto __ret = ((::RenderBuffer*)NativePtr)->createRenderTexture(arg0);
    return gcnew Flood::Texture((::Texture*)__ret.get());
}

void Flood::RenderBuffer::AttachRenderTexture(Flood::Texture^ tex)
{
    auto arg0 = (::Texture*)tex->NativePtr;
    ((::RenderBuffer*)NativePtr)->attachRenderTexture(arg0);
}

bool Flood::RenderBuffer::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<RenderBuffer^>(object);

    if (!obj) return false;
    return Instance == obj->Instance;
}

int Flood::RenderBuffer::GetHashCode()
{
    return (int)NativePtr;
}

Flood::Settings Flood::RenderBuffer::Settings::get()
{
    auto &__ret = ((::RenderBuffer*)NativePtr)->getSettings();
    return Flood::Settings((::Settings*)&__ret);
}

