/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Engine/Texture/TextureAtlas.h>
#include "Image.h"
#include "ResourceHandle.h"
#include "Vector.h"

namespace Flood
{
    enum struct PixelFormat;
    ref class Handle;
    ref class Image;
    ref class MaxRectsBinPack;
    ref class TextureAtlas;
    value struct Rectangle;
    value struct SubTexture;
    value struct Vector2;

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

    public ref class TextureAtlas
    {
    public:
        property ::TextureAtlas* NativePtr;

        TextureAtlas(::TextureAtlas* native);
        TextureAtlas(System::IntPtr native);
        TextureAtlas(unsigned int maxSize, Flood::PixelFormat pixelFormat);
        bool AddImage(Flood::ResourceHandle<Flood::Image^> imageHandle);
        bool GetImageSubTexture(Flood::ResourceHandle<Flood::Image^> imageHandle, [System::Runtime::InteropServices::Out] Flood::SubTexture% subTexture);
        Flood::ResourceHandle<Flood::Image^> GetAtlasImageHandle();
    };
}
