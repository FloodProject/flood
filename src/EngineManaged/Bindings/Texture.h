/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Graphics/Texture.h>
#include "Reflection.h"
#include "Memory.h"
#include "Vector.h"
#include "Color.h"
#include "Resource.h"

namespace Flood
{
    enum struct TextureFilterMode;
    enum struct TextureMipMode;
    enum struct TextureWrapMode;
    enum struct TextureTarget;
    ref class Texture;
    enum struct PixelFormat;
    ref class Image;
    ref class Class;
    ref class Allocator;
    ref class Field;
    ref class Type;
    enum struct TypeKind : unsigned char;
    ref class ReflectionContext;
    enum struct ReflectionWalkType : unsigned char;
    enum struct FieldQualifier : unsigned short;
    enum struct ResourceGroup;
    value struct Color;
    value struct Vector3;
    value struct Vector2i;
    ref class TextureUnit;
    ref class Handle;
    ref class RefPtr;

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
        property unsigned int Id;
        property Flood::TextureTarget Target;
        property bool Uploaded;
        property unsigned short Width;
        property unsigned short Height;
        property Flood::PixelFormat Format;
        property float AnisotropicFilter;
        property Flood::Image^ Image;
        void Alloc(Flood::Vector2i size, Flood::PixelFormat _207);
        unsigned int GetId();
        Flood::PixelFormat GetPixelFormat();
        Flood::Image^ GetImage();
        void SetImage(Flood::Image^ image);
        Flood::Image^ ReadImage();
        unsigned int GetExpectedSize();
    };

    public ref class TextureUnit
    {
    public:
        property ::TextureUnit* NativePtr;

        TextureUnit(::TextureUnit* native);
        TextureUnit(System::IntPtr native);
        TextureUnit();
        property unsigned char Unit;
        property bool OverrideModes;
        property Flood::TextureFilterMode Filter;
        property Flood::TextureMipMode Mip;
        property Flood::TextureWrapMode Wrap;
        Flood::TextureFilterMode GetFilterMode();
        Flood::TextureMipMode GetMipMode();
        Flood::TextureWrapMode GetWrapMode();
        void SetFilterMode(Flood::TextureFilterMode _209);
        void SetMipMode(Flood::TextureMipMode _210);
        void SetWrapMode(Flood::TextureWrapMode _211);
    };
}
