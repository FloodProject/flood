/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Engine/Texture/TextureAtlas.h>
#include "ResourceHandle.h"

namespace Flood
{
    ref class CompareHandle;
    ref class Handle;
    ref class Image;
    ref class MaxRectsBinPack;
    ref class Rect;
    ref class SubTexture;
    ref class TextureAtlas;

    public ref class CompareHandle
    {
    public:
        property ::CompareHandle* NativePtr;

        CompareHandle(::CompareHandle* native);
        CompareHandle(System::IntPtr native);
        bool operator()(Flood::ResourceHandle<Flood::Image^> h1, Flood::ResourceHandle<Flood::Image^> h2);
    };

    public ref class SubTexture
    {
    public:
        property ::SubTexture* NativePtr;

        SubTexture(::SubTexture* native);
        SubTexture(System::IntPtr native);
        property Flood::Rect^ Rect
        {
            Flood::Rect^ get();
            void set(Flood::Rect^);
        }
        property bool IsRotated
        {
            bool get();
            void set(bool);
        }
    };

    public ref class TextureAtlas
    {
    public:
        property ::TextureAtlas* NativePtr;

        TextureAtlas(::TextureAtlas* native);
        TextureAtlas(System::IntPtr native);
        TextureAtlas(unsigned int maxSize);
        bool AddImage(Flood::ResourceHandle<Flood::Image^> imageHandle);
        bool GetImageSubTexture(Flood::ResourceHandle<Flood::Image^> imageHandle, Flood::SubTexture^ subTexture);
        Flood::ResourceHandle<Flood::Image^> GetAtlasImageHandle();
        static property unsigned int DefaultSize;
    };
}
