/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "RenderBackend.h"
#include "Color.h"
#include "Image.h"
#include "IndexBuffer.h"
#include "RenderBatch.h"
#include "RenderBuffer.h"
#include "RenderQueue.h"
#include "RenderTarget.h"
#include "RenderView.h"
#include "Texture.h"
#include "VertexBuffer.h"

using namespace System;
using namespace System::Runtime::InteropServices;

Flood::RenderBackend::RenderBackend(::RenderBackend* native)
{
    NativePtr = native;
}

Flood::RenderBackend::RenderBackend(System::IntPtr native)
{
    auto __native = (::RenderBackend*)native.ToPointer();
    NativePtr = __native;
}

bool Flood::RenderBackend::Init()
{
    auto __ret = ((::RenderBackend*)NativePtr)->init();
    return __ret;
}

void Flood::RenderBackend::Cleanup()
{
    ((::RenderBackend*)NativePtr)->cleanup();
}

void Flood::RenderBackend::RenderBatch(Flood::RenderBatch^ _6)
{
    auto arg0 = (::RenderBatch*)_6->NativePtr;
    ((::RenderBackend*)NativePtr)->renderBatch(arg0);
}

void Flood::RenderBackend::SetupRenderState(Flood::RenderState _7, bool bindUniforms)
{
    auto _marshal0 = ::RenderState();
    if (_7.Renderable != nullptr)
        _marshal0.renderable = (::RenderBatch*)_7.Renderable->NativePtr;
    if (_7.Material != nullptr)
        _marshal0.material = (::Material*)_7.Material->NativePtr;
    auto _marshal1 = ::Matrix4x3();
    _marshal1.m11 = _7.ModelMatrix.M11;
    _marshal1.m12 = _7.ModelMatrix.M12;
    _marshal1.m13 = _7.ModelMatrix.M13;
    _marshal1.m21 = _7.ModelMatrix.M21;
    _marshal1.m22 = _7.ModelMatrix.M22;
    _marshal1.m23 = _7.ModelMatrix.M23;
    _marshal1.m31 = _7.ModelMatrix.M31;
    _marshal1.m32 = _7.ModelMatrix.M32;
    _marshal1.m33 = _7.ModelMatrix.M33;
    _marshal1.tx = _7.ModelMatrix.Tx;
    _marshal1.ty = _7.ModelMatrix.Ty;
    _marshal1.tz = _7.ModelMatrix.Tz;
    _marshal0.modelMatrix = _marshal1;
    _marshal0.priority = (::int32)(::int32_t)_7.Priority;
    auto arg0 = _marshal0;
    ((::RenderBackend*)NativePtr)->setupRenderState(arg0, bindUniforms);
}

void Flood::RenderBackend::UnsetupRenderState(Flood::RenderState _8)
{
    auto _marshal0 = ::RenderState();
    if (_8.Renderable != nullptr)
        _marshal0.renderable = (::RenderBatch*)_8.Renderable->NativePtr;
    if (_8.Material != nullptr)
        _marshal0.material = (::Material*)_8.Material->NativePtr;
    auto _marshal1 = ::Matrix4x3();
    _marshal1.m11 = _8.ModelMatrix.M11;
    _marshal1.m12 = _8.ModelMatrix.M12;
    _marshal1.m13 = _8.ModelMatrix.M13;
    _marshal1.m21 = _8.ModelMatrix.M21;
    _marshal1.m22 = _8.ModelMatrix.M22;
    _marshal1.m23 = _8.ModelMatrix.M23;
    _marshal1.m31 = _8.ModelMatrix.M31;
    _marshal1.m32 = _8.ModelMatrix.M32;
    _marshal1.m33 = _8.ModelMatrix.M33;
    _marshal1.tx = _8.ModelMatrix.Tx;
    _marshal1.ty = _8.ModelMatrix.Ty;
    _marshal1.tz = _8.ModelMatrix.Tz;
    _marshal0.modelMatrix = _marshal1;
    _marshal0.priority = (::int32)(::int32_t)_8.Priority;
    auto arg0 = _marshal0;
    ((::RenderBackend*)NativePtr)->unsetupRenderState(arg0);
}

Flood::Color Flood::RenderBackend::GetPixel(unsigned short x, unsigned short y)
{
    auto arg0 = (::uint16)(::uint16_t)x;
    auto arg1 = (::uint16)(::uint16_t)y;
    auto __ret = ((::RenderBackend*)NativePtr)->getPixel(arg0, arg1);
    return Flood::Color((::Color*)&__ret);
}

void Flood::RenderBackend::ClearRenderView(Flood::RenderView^ _10)
{
    auto arg0 = (::RenderView*)_10->NativePtr;
    ((::RenderBackend*)NativePtr)->clearRenderView(arg0);
}

Flood::VertexBuffer^ Flood::RenderBackend::CreateVertexBuffer()
{
    auto __ret = ((::RenderBackend*)NativePtr)->createVertexBuffer();
    if (__ret == nullptr) return nullptr;
    return (__ret == nullptr) ? nullptr : gcnew Flood::VertexBuffer((::VertexBuffer*)__ret);
}

void Flood::RenderBackend::ReleaseVertexBuffer(Flood::VertexBuffer^ _11)
{
    auto arg0 = (::VertexBuffer*)_11->NativePtr;
    ((::RenderBackend*)NativePtr)->releaseVertexBuffer(arg0);
}

void Flood::RenderBackend::BindVertexBuffer(Flood::VertexBuffer^ _12)
{
    auto arg0 = (::VertexBuffer*)_12->NativePtr;
    ((::RenderBackend*)NativePtr)->bindVertexBuffer(arg0);
}

void Flood::RenderBackend::UnbindVertexBuffer(Flood::VertexBuffer^ _13)
{
    auto arg0 = (::VertexBuffer*)_13->NativePtr;
    ((::RenderBackend*)NativePtr)->unbindVertexBuffer(arg0);
}

void Flood::RenderBackend::BuildVertexBuffer(Flood::VertexBuffer^ _14)
{
    auto arg0 = (::VertexBuffer*)_14->NativePtr;
    ((::RenderBackend*)NativePtr)->buildVertexBuffer(arg0);
}

Flood::IndexBuffer^ Flood::RenderBackend::CreateIndexBuffer()
{
    auto __ret = ((::RenderBackend*)NativePtr)->createIndexBuffer();
    if (__ret == nullptr) return nullptr;
    return (__ret == nullptr) ? nullptr : gcnew Flood::IndexBuffer((::IndexBuffer*)__ret);
}

void Flood::RenderBackend::ReleaseIndexBuffer(Flood::IndexBuffer^ _16)
{
    auto arg0 = (::IndexBuffer*)_16->NativePtr;
    ((::RenderBackend*)NativePtr)->releaseIndexBuffer(arg0);
}

void Flood::RenderBackend::BindIndexBuffer(Flood::IndexBuffer^ _17)
{
    auto arg0 = (::IndexBuffer*)_17->NativePtr;
    ((::RenderBackend*)NativePtr)->bindIndexBuffer(arg0);
}

void Flood::RenderBackend::UnbindIndexBuffer(Flood::IndexBuffer^ _18)
{
    auto arg0 = (::IndexBuffer*)_18->NativePtr;
    ((::RenderBackend*)NativePtr)->unbindIndexBuffer(arg0);
}

void Flood::RenderBackend::BuildIndexBuffer(Flood::IndexBuffer^ _19)
{
    auto arg0 = (::IndexBuffer*)_19->NativePtr;
    ((::RenderBackend*)NativePtr)->buildIndexBuffer(arg0);
}

Flood::RenderBuffer^ Flood::RenderBackend::CreateRenderBuffer(Flood::Settings _20)
{
    auto _marshal0 = ::Settings();
    _marshal0.width = (::uint16)(::uint16_t)_20.Width;
    _marshal0.height = (::uint16)(::uint16_t)_20.Height;
    auto arg0 = _marshal0;
    auto __ret = ((::RenderBackend*)NativePtr)->createRenderBuffer(arg0);
    if (__ret == nullptr) return nullptr;
    return (__ret == nullptr) ? nullptr : gcnew Flood::RenderBuffer((::RenderBuffer*)__ret);
}

Flood::Texture^ Flood::RenderBackend::CreateTexture()
{
    auto __ret = ((::RenderBackend*)NativePtr)->createTexture();
    if (__ret == nullptr) return nullptr;
    return (__ret == nullptr) ? nullptr : gcnew Flood::Texture((::Texture*)__ret);
}

void Flood::RenderBackend::ReleaseTexture(Flood::Texture^ _21)
{
    auto arg0 = (::Texture*)_21->NativePtr;
    ((::RenderBackend*)NativePtr)->releaseTexture(arg0);
}

void Flood::RenderBackend::UploadTexture(Flood::Texture^ _22)
{
    auto arg0 = (::Texture*)_22->NativePtr;
    ((::RenderBackend*)NativePtr)->uploadTexture(arg0);
}

void Flood::RenderBackend::ConfigureTexture(Flood::Texture^ _23)
{
    auto arg0 = (::Texture*)_23->NativePtr;
    ((::RenderBackend*)NativePtr)->configureTexture(arg0);
}

void Flood::RenderBackend::BindTexture(Flood::Texture^ _24)
{
    auto arg0 = (::Texture*)_24->NativePtr;
    ((::RenderBackend*)NativePtr)->bindTexture(arg0);
}

void Flood::RenderBackend::UnbindTexture(Flood::Texture^ _25)
{
    auto arg0 = (::Texture*)_25->NativePtr;
    ((::RenderBackend*)NativePtr)->unbindTexture(arg0);
}

Flood::Image^ Flood::RenderBackend::ReadTexture(Flood::Texture^ _26)
{
    auto arg0 = (::Texture*)_26->NativePtr;
    auto __ret = ((::RenderBackend*)NativePtr)->readTexture(arg0);
    if (__ret == nullptr) return nullptr;
    return (__ret == nullptr) ? nullptr : gcnew Flood::Image((::Image*)__ret);
}

void Flood::RenderBackend::SetupTextureUnit(Flood::Texture^ texture, Flood::TextureUnit^ unit)
{
    auto arg0 = (::Texture*)texture->NativePtr;
    auto &arg1 = *(::TextureUnit*)unit->NativePtr;
    ((::RenderBackend*)NativePtr)->setupTextureUnit(arg0, arg1);
}

void Flood::RenderBackend::UndoTextureUnit(Flood::Texture^ texture, Flood::TextureUnit^ unit)
{
    auto arg0 = (::Texture*)texture->NativePtr;
    auto &arg1 = *(::TextureUnit*)unit->NativePtr;
    ((::RenderBackend*)NativePtr)->undoTextureUnit(arg0, arg1);
}

Flood::RenderBackend::RenderBackend()
{
}

bool Flood::RenderBackend::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<RenderBackend^>(object);

    if (!obj) return false;
    return __Instance == obj->__Instance;
}

int Flood::RenderBackend::GetHashCode()
{
    return (int)NativePtr;
}

System::IntPtr Flood::RenderBackend::__Instance::get()
{
    return System::IntPtr(NativePtr);
}

void Flood::RenderBackend::__Instance::set(System::IntPtr object)
{
    NativePtr = (::RenderBackend*)object.ToPointer();
}

void Flood::RenderBackend::ClearColor::set(Flood::Color color)
{
    auto _marshal0 = ::Color();
    _marshal0.r = (::byte)(::uint8)(::uint8_t)color.R;
    _marshal0.g = (::byte)(::uint8)(::uint8_t)color.G;
    _marshal0.b = (::byte)(::uint8)(::uint8_t)color.B;
    _marshal0.a = (::byte)(::uint8)(::uint8_t)color.A;
    auto arg0 = _marshal0;
    ((::RenderBackend*)NativePtr)->setClearColor(arg0);
}

void Flood::RenderBackend::upRenderView::set(Flood::RenderView^ _9)
{
    auto arg0 = (::RenderView*)_9->NativePtr;
    ((::RenderBackend*)NativePtr)->setupRenderView(arg0);
}

void Flood::RenderBackend::upVertexBuffer::set(Flood::VertexBuffer^ _15)
{
    auto arg0 = (::VertexBuffer*)_15->NativePtr;
    ((::RenderBackend*)NativePtr)->setupVertexBuffer(arg0);
}

