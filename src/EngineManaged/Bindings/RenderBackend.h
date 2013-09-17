/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "CppSharp.h"
#include <Graphics/RenderBackend.h>

namespace Flood
{
    ref class Image;
    ref class IndexBuffer;
    ref class RenderBackend;
    ref class RenderBatch;
    ref class RenderBuffer;
    ref class RenderView;
    ref class Texture;
    ref class TextureUnit;
    ref class VertexBuffer;
    value struct Color;
    value struct RenderState;
    value struct Settings;

    public ref class RenderBackend : ICppInstance
    {
    public:

        property ::RenderBackend* NativePtr;
        property System::IntPtr Instance
        {
            virtual System::IntPtr get();
            virtual void set(System::IntPtr instance);
        }

        RenderBackend(::RenderBackend* native);
        RenderBackend(System::IntPtr native);
        RenderBackend();

        property Flood::Color ClearColor
        {
            void set(Flood::Color);
        }
        property Flood::RenderView^ upRenderView
        {
            void set(Flood::RenderView^);
        }
        property Flood::VertexBuffer^ upVertexBuffer
        {
            void set(Flood::VertexBuffer^);
        }
        virtual bool Init();

        virtual void Cleanup();

        virtual void RenderBatch(Flood::RenderBatch^ _1);

        virtual void SetupRenderState(Flood::RenderState _2, bool bindUniforms);

        virtual void UnsetupRenderState(Flood::RenderState _3);

        virtual Flood::Color GetPixel(unsigned short x, unsigned short y);

        virtual void ClearRenderView(Flood::RenderView^ _5);

        virtual Flood::VertexBuffer^ CreateVertexBuffer();

        virtual void ReleaseVertexBuffer(Flood::VertexBuffer^ _6);

        virtual void BindVertexBuffer(Flood::VertexBuffer^ _7);

        virtual void UnbindVertexBuffer(Flood::VertexBuffer^ _8);

        virtual void BuildVertexBuffer(Flood::VertexBuffer^ _9);

        virtual Flood::IndexBuffer^ CreateIndexBuffer();

        virtual void ReleaseIndexBuffer(Flood::IndexBuffer^ _11);

        virtual void BindIndexBuffer(Flood::IndexBuffer^ _12);

        virtual void UnbindIndexBuffer(Flood::IndexBuffer^ _13);

        virtual void BuildIndexBuffer(Flood::IndexBuffer^ _14);

        virtual Flood::RenderBuffer^ CreateRenderBuffer(Flood::Settings _15);

        virtual Flood::Texture^ CreateTexture();

        virtual void ReleaseTexture(Flood::Texture^ _16);

        virtual void UploadTexture(Flood::Texture^ _17);

        virtual void ConfigureTexture(Flood::Texture^ _18);

        virtual void BindTexture(Flood::Texture^ _19);

        virtual void UnbindTexture(Flood::Texture^ _20);

        virtual Flood::Image^ ReadTexture(Flood::Texture^ _21);

        virtual void SetupTextureUnit(Flood::Texture^ texture, Flood::TextureUnit^ unit);

        virtual void UndoTextureUnit(Flood::Texture^ texture, Flood::TextureUnit^ unit);

        virtual bool Equals(System::Object^ object) override;

        virtual int GetHashCode() override;

    };
}
