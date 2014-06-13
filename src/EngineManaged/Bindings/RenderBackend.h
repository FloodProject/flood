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
    ref class RenderCapabilities;
    ref class RenderView;
    ref class Shader;
    ref class ShaderProgram;
    ref class Texture;
    ref class TextureUnit;
    ref class VertexBuffer;
    value struct Color;
    value struct RenderState;
    value struct Settings;
}

namespace Flood
{
    public ref class RenderBackend : ICppInstance
    {
    public:

        property ::RenderBackend* NativePtr;
        property System::IntPtr __Instance
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

        virtual void RenderBatch(Flood::RenderBatch^ _6);

        virtual void SetupRenderState(Flood::RenderState _7, bool bindUniforms);

        virtual void UnsetupRenderState(Flood::RenderState _8);

        virtual Flood::Color GetPixel(unsigned short x, unsigned short y);

        virtual void ClearRenderView(Flood::RenderView^ _10);

        virtual Flood::VertexBuffer^ CreateVertexBuffer();

        virtual void ReleaseVertexBuffer(Flood::VertexBuffer^ _11);

        virtual void BindVertexBuffer(Flood::VertexBuffer^ _12);

        virtual void UnbindVertexBuffer(Flood::VertexBuffer^ _13);

        virtual void BuildVertexBuffer(Flood::VertexBuffer^ _14);

        virtual Flood::IndexBuffer^ CreateIndexBuffer();

        virtual void ReleaseIndexBuffer(Flood::IndexBuffer^ _16);

        virtual void BindIndexBuffer(Flood::IndexBuffer^ _17);

        virtual void UnbindIndexBuffer(Flood::IndexBuffer^ _18);

        virtual void BuildIndexBuffer(Flood::IndexBuffer^ _19);

        virtual Flood::RenderBuffer^ CreateRenderBuffer(Flood::Settings _20);

        virtual Flood::Texture^ CreateTexture();

        virtual void ReleaseTexture(Flood::Texture^ _21);

        virtual void UploadTexture(Flood::Texture^ _22);

        virtual void ConfigureTexture(Flood::Texture^ _23);

        virtual void BindTexture(Flood::Texture^ _24);

        virtual void UnbindTexture(Flood::Texture^ _25);

        virtual Flood::Image^ ReadTexture(Flood::Texture^ _26);

        virtual void SetupTextureUnit(Flood::Texture^ texture, Flood::TextureUnit^ unit);

        virtual void UndoTextureUnit(Flood::Texture^ texture, Flood::TextureUnit^ unit);

        virtual bool Equals(System::Object^ object) override;

        virtual int GetHashCode() override;
    };
}
