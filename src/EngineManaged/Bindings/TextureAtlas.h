/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "CppSharp.h"
#include <Engine/Texture/TextureAtlas.h>
#include "Image.h"
#include "ResourceHandle.h"
#include "Vector.h"

namespace Flood
{
    enum struct PixelFormat;
    ref class TextureAtlas;
    value struct Rectangle;
    value struct SubTexture;
}

namespace Flood
{
    public value struct SubTexture
    {
    public:

        SubTexture(::SubTexture* native);
        SubTexture(System::IntPtr native);
        Flood::Vector2 LeftTopUV;
        Flood::Vector2 RightTopUV;
        Flood::Vector2 RightBottomUV;
        Flood::Vector2 LeftBottomUV;
        Flood::TextureAtlas^ Atlas;
    };

    public ref class TextureAtlas : ICppInstance
    {
    public:

        property ::TextureAtlas* NativePtr;
        property System::IntPtr Instance
        {
            virtual System::IntPtr get();
            virtual void set(System::IntPtr instance);
        }

        TextureAtlas(::TextureAtlas* native);
        TextureAtlas(System::IntPtr native);
        TextureAtlas(unsigned int maxSize, Flood::PixelFormat pixelFormat);

        property Flood::ResourceHandle<Flood::Image^> AtlasImageHandle
        {
            Flood::ResourceHandle<Flood::Image^> get();
        }

        bool AddImage(Flood::ResourceHandle<Flood::Image^> imageHandle);

        bool GetImageSubTexture(Flood::ResourceHandle<Flood::Image^> imageHandle, [System::Runtime::InteropServices::Out] Flood::SubTexture% subTexture);

        virtual bool Equals(System::Object^ object) override;

        virtual int GetHashCode() override;
    };
}
