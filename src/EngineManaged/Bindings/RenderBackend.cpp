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

void Flood::RenderBackend::RenderBatch(Flood::RenderBatch^ _1)
{
    auto arg0 = (::RenderBatch*)_1->NativePtr;
    ((::RenderBackend*)NativePtr)->renderBatch(arg0);
}

void Flood::RenderBackend::SetupRenderState(Flood::RenderState _2, bool bindUniforms)
{
    auto _marshal0 = ::RenderState();
    if (_2.Renderable != nullptr)
        _marshal0.renderable = (::RenderBatch*)_2.Renderable->NativePtr;
    if (_2.Material != nullptr)
        _marshal0.material = (::Material*)_2.Material->NativePtr;
    auto _marshal1 = ::Matrix4x3();
    _marshal1.m11 = _2.ModelMatrix.M11;
    _marshal1.m12 = _2.ModelMatrix.M12;
    _marshal1.m13 = _2.ModelMatrix.M13;
    _marshal1.m21 = _2.ModelMatrix.M21;
    _marshal1.m22 = _2.ModelMatrix.M22;
    _marshal1.m23 = _2.ModelMatrix.M23;
    _marshal1.m31 = _2.ModelMatrix.M31;
    _marshal1.m32 = _2.ModelMatrix.M32;
    _marshal1.m33 = _2.ModelMatrix.M33;
    _marshal1.tx = _2.ModelMatrix.Tx;
    _marshal1.ty = _2.ModelMatrix.Ty;
    _marshal1.tz = _2.ModelMatrix.Tz;
    _marshal0.modelMatrix = _marshal1;
    _marshal0.priority = (::int32)(::int32_t)_2.Priority;
    auto arg0 = _marshal0;
    ((::RenderBackend*)NativePtr)->setupRenderState(arg0, bindUniforms);
}

void Flood::RenderBackend::UnsetupRenderState(Flood::RenderState _3)
{
    auto _marshal0 = ::RenderState();
    if (_3.Renderable != nullptr)
        _marshal0.renderable = (::RenderBatch*)_3.Renderable->NativePtr;
    if (_3.Material != nullptr)
        _marshal0.material = (::Material*)_3.Material->NativePtr;
    auto _marshal1 = ::Matrix4x3();
    _marshal1.m11 = _3.ModelMatrix.M11;
    _marshal1.m12 = _3.ModelMatrix.M12;
    _marshal1.m13 = _3.ModelMatrix.M13;
    _marshal1.m21 = _3.ModelMatrix.M21;
    _marshal1.m22 = _3.ModelMatrix.M22;
    _marshal1.m23 = _3.ModelMatrix.M23;
    _marshal1.m31 = _3.ModelMatrix.M31;
    _marshal1.m32 = _3.ModelMatrix.M32;
    _marshal1.m33 = _3.ModelMatrix.M33;
    _marshal1.tx = _3.ModelMatrix.Tx;
    _marshal1.ty = _3.ModelMatrix.Ty;
    _marshal1.tz = _3.ModelMatrix.Tz;
    _marshal0.modelMatrix = _marshal1;
    _marshal0.priority = (::int32)(::int32_t)_3.Priority;
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

void Flood::RenderBackend::ClearRenderView(Flood::RenderView^ _5)
{
    auto arg0 = (::RenderView*)_5->NativePtr;
    ((::RenderBackend*)NativePtr)->clearRenderView(arg0);
}

Flood::VertexBuffer^ Flood::RenderBackend::CreateVertexBuffer()
{
    auto __ret = ((::RenderBackend*)NativePtr)->createVertexBuffer();
    if (__ret == nullptr) return nullptr;
    return gcnew Flood::VertexBuffer((::VertexBuffer*)__ret);
}

void Flood::RenderBackend::ReleaseVertexBuffer(Flood::VertexBuffer^ _6)
{
    auto arg0 = (::VertexBuffer*)_6->NativePtr;
    ((::RenderBackend*)NativePtr)->releaseVertexBuffer(arg0);
}

void Flood::RenderBackend::BindVertexBuffer(Flood::VertexBuffer^ _7)
{
    auto arg0 = (::VertexBuffer*)_7->NativePtr;
    ((::RenderBackend*)NativePtr)->bindVertexBuffer(arg0);
}

void Flood::RenderBackend::UnbindVertexBuffer(Flood::VertexBuffer^ _8)
{
    auto arg0 = (::VertexBuffer*)_8->NativePtr;
    ((::RenderBackend*)NativePtr)->unbindVertexBuffer(arg0);
}

void Flood::RenderBackend::BuildVertexBuffer(Flood::VertexBuffer^ _9)
{
    auto arg0 = (::VertexBuffer*)_9->NativePtr;
    ((::RenderBackend*)NativePtr)->buildVertexBuffer(arg0);
}

Flood::IndexBuffer^ Flood::RenderBackend::CreateIndexBuffer()
{
    auto __ret = ((::RenderBackend*)NativePtr)->createIndexBuffer();
    if (__ret == nullptr) return nullptr;
    return gcnew Flood::IndexBuffer((::IndexBuffer*)__ret);
}

void Flood::RenderBackend::ReleaseIndexBuffer(Flood::IndexBuffer^ _11)
{
    auto arg0 = (::IndexBuffer*)_11->NativePtr;
    ((::RenderBackend*)NativePtr)->releaseIndexBuffer(arg0);
}

void Flood::RenderBackend::BindIndexBuffer(Flood::IndexBuffer^ _12)
{
    auto arg0 = (::IndexBuffer*)_12->NativePtr;
    ((::RenderBackend*)NativePtr)->bindIndexBuffer(arg0);
}

void Flood::RenderBackend::UnbindIndexBuffer(Flood::IndexBuffer^ _13)
{
    auto arg0 = (::IndexBuffer*)_13->NativePtr;
    ((::RenderBackend*)NativePtr)->unbindIndexBuffer(arg0);
}

void Flood::RenderBackend::BuildIndexBuffer(Flood::IndexBuffer^ _14)
{
    auto arg0 = (::IndexBuffer*)_14->NativePtr;
    ((::RenderBackend*)NativePtr)->buildIndexBuffer(arg0);
}

Flood::RenderBuffer^ Flood::RenderBackend::CreateRenderBuffer(Flood::Settings _15)
{
    auto _marshal0 = ::Settings();
    _marshal0.width = (::uint16)(::uint16_t)_15.Width;
    _marshal0.height = (::uint16)(::uint16_t)_15.Height;
    auto arg0 = _marshal0;
    auto __ret = ((::RenderBackend*)NativePtr)->createRenderBuffer(arg0);
    if (__ret == nullptr) return nullptr;
    return gcnew Flood::RenderBuffer((::RenderBuffer*)__ret);
}

Flood::Texture^ Flood::RenderBackend::CreateTexture()
{
    auto __ret = ((::RenderBackend*)NativePtr)->createTexture();
    if (__ret == nullptr) return nullptr;
    return gcnew Flood::Texture((::Texture*)__ret);
}

void Flood::RenderBackend::ReleaseTexture(Flood::Texture^ _16)
{
    auto arg0 = (::Texture*)_16->NativePtr;
    ((::RenderBackend*)NativePtr)->releaseTexture(arg0);
}

void Flood::RenderBackend::UploadTexture(Flood::Texture^ _17)
{
    auto arg0 = (::Texture*)_17->NativePtr;
    ((::RenderBackend*)NativePtr)->uploadTexture(arg0);
}

void Flood::RenderBackend::ConfigureTexture(Flood::Texture^ _18)
{
    auto arg0 = (::Texture*)_18->NativePtr;
    ((::RenderBackend*)NativePtr)->configureTexture(arg0);
}

void Flood::RenderBackend::BindTexture(Flood::Texture^ _19)
{
    auto arg0 = (::Texture*)_19->NativePtr;
    ((::RenderBackend*)NativePtr)->bindTexture(arg0);
}

void Flood::RenderBackend::UnbindTexture(Flood::Texture^ _20)
{
    auto arg0 = (::Texture*)_20->NativePtr;
    ((::RenderBackend*)NativePtr)->unbindTexture(arg0);
}

Flood::Image^ Flood::RenderBackend::ReadTexture(Flood::Texture^ _21)
{
    auto arg0 = (::Texture*)_21->NativePtr;
    auto __ret = ((::RenderBackend*)NativePtr)->readTexture(arg0);
    if (__ret == nullptr) return nullptr;
    return gcnew Flood::Image((::Image*)__ret);
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
    return Instance == obj->Instance;
}

int Flood::RenderBackend::GetHashCode()
{
    return (int)NativePtr;
}

System::IntPtr Flood::RenderBackend::Instance::get()
{
    return System::IntPtr(NativePtr);
}

void Flood::RenderBackend::Instance::set(System::IntPtr object)
{
    NativePtr = (::RenderBackend*)object.ToPointer();
}

void Flood::RenderBackend::ClearColor::set(Flood::Color value)
{
    auto color = value;
    auto _marshal0 = ::Color();
    _marshal0.r = (::byte)(::uint8)(::uint8_t)color.R;
    _marshal0.g = (::byte)(::uint8)(::uint8_t)color.G;
    _marshal0.b = (::byte)(::uint8)(::uint8_t)color.B;
    _marshal0.a = (::byte)(::uint8)(::uint8_t)color.A;
    auto arg0 = _marshal0;
    ((::RenderBackend*)NativePtr)->setClearColor(arg0);
}

void Flood::RenderBackend::upRenderView::set(Flood::RenderView^ value)
{
    auto _4 = value;
    auto arg0 = (::RenderView*)_4->NativePtr;
    ((::RenderBackend*)NativePtr)->setupRenderView(arg0);
}

void Flood::RenderBackend::upVertexBuffer::set(Flood::VertexBuffer^ value)
{
    auto _10 = value;
    auto arg0 = (::VertexBuffer*)_10->NativePtr;
    ((::RenderBackend*)NativePtr)->setupVertexBuffer(arg0);
}

