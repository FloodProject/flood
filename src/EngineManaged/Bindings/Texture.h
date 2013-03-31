/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Graphics/Texture.h>
#include "Image.h"
#include "ResourceHandle.h"

namespace Flood
{
    enum struct PixelFormat;
    enum struct TextureFilterMode;
    enum struct TextureMipMode;
    enum struct TextureTarget;
    enum struct TextureWrapMode;
    ref class Handle;
    ref class Image;
    ref class RefPtr;
    ref class Texture;
    ref class TextureUnit;
    value struct Vector2i;

    public enum struct TextureFilterMode
    {
        Nearest = 0,
        Linear = 1
    };

    public enum struct TextureMipMode
    {
        Nearest = 0,
        Linear = 1
    };

    public enum struct TextureWrapMode
    {
        Repeat = 0,
        Clamp = 1,
        ClampToBorder = 2,
        ClampToEdge = 3
    };

    /// <summary>
    /// Represents a texture on the rendering API. This will be basically an the
    /// same content of a resource image stored on the graphics card, but it is not
    /// guaranteed that the GPU internal format will be the same. You can also
    /// create textures that are not backed by an image.
    /// </summary>
    public enum struct TextureTarget
    {
        Target2D = 0,
        Target3D = 1
    };

    public ref class Texture
    {
    public:
        property ::Texture* NativePtr;

        Texture(::Texture* native);
        Texture(System::IntPtr native);
        Texture();
        ~Texture(){}
        property unsigned int Id
        {
            unsigned int get();
            void set(unsigned int);
        }
        property Flood::TextureTarget Target
        {
            Flood::TextureTarget get();
            void set(Flood::TextureTarget);
        }
        property int Width
        {
            int get();
            void set(int);
        }
        property int Height
        {
            int get();
            void set(int);
        }
        property Flood::PixelFormat Format
        {
            Flood::PixelFormat get();
            void set(Flood::PixelFormat);
        }
        property float AnisotropicFilter
        {
            float get();
            void set(float);
        }
        void Alloc(Flood::Vector2i size, Flood::PixelFormat _0);
        unsigned int GetId();
        Flood::PixelFormat GetPixelFormat();
        Flood::ResourceHandle<Flood::Image^> GetImage();
        void SetImage(Flood::ResourceHandle<Flood::Image^> image);
        Flood::Image^ ReadImage();
        int GetExpectedSize();
        bool IsUploaded();
        void SetUploaded();
    };

    public ref class TextureUnit
    {
    public:
        property ::TextureUnit* NativePtr;

        TextureUnit(::TextureUnit* native);
        TextureUnit(System::IntPtr native);
        TextureUnit();
        property unsigned char Unit
        {
            unsigned char get();
            void set(unsigned char);
        }
        property Flood::ResourceHandle<Flood::Image^> Image
        {
            Flood::ResourceHandle<Flood::Image^> get();
            void set(Flood::ResourceHandle<Flood::Image^>);
        }
        property bool OverrideModes
        {
            bool get();
            void set(bool);
        }
        Flood::TextureFilterMode GetFilterMode();
        Flood::TextureMipMode GetMipMode();
        Flood::TextureWrapMode GetWrapMode();
        void SetFilterMode(Flood::TextureFilterMode _0);
        void SetMipMode(Flood::TextureMipMode _0);
        void SetWrapMode(Flood::TextureWrapMode _0);
    };
}
