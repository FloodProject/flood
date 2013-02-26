/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Graphics/Resources/Image.h>
#include "Reflection.h"
#include "Memory.h"
#include "Vector.h"
#include "Color.h"
#include "Resource.h"

namespace Flood
{
    enum struct PixelFormat;
    ref class Enum;
    enum struct PrimitiveTypeKind : unsigned char;
    ref class Class;
    ref class Allocator;
    ref class Field;
    ref class Type;
    enum struct TypeKind : unsigned char;
    ref class ReflectionContext;
    enum struct ReflectionWalkType : unsigned char;
    enum struct FieldQualifier : unsigned short;
    ref class Handle;
    ref class Image;
    enum struct ResourceGroup;
    value struct Color;
    value struct Vector3;
    ref class Stream;
    ref class ImageWriter;
    ref class RefPtr;

    /// <summary>
    /// Possible pixel formats for an image.
    /// </summary>
    public enum struct PixelFormat
    {
        R8G8B8A8 = 0,
        R8G8B8 = 1,
        B8G8R8 = 2,
        B8G8R8A8 = 3,
        DXT1 = 4,
        DXT1a = 5,
        DXT3 = 6,
        DXT5 = 7,
        DXT5nm = 8,
        Depth = 9,
        Unknown = 10
    };

    /// <summary>
    /// Represents an image in the engine. An image usually holds uncompressed
    /// image data, stored in an pixel array in a given pixel format specified at
    /// creation time.
    /// </summary>
    public ref class Image : Resource
    {
    public:
        property ::Image* NativePtr;

        Image(::Image* native);
        Image(System::IntPtr native);
        Image();
        Image(unsigned int width, unsigned int height, Flood::PixelFormat format);
        property unsigned int Width;
        property unsigned int Height;
        property Flood::PixelFormat Format;
        Flood::Class^ GetType();
        Flood::Class^ GetStaticType();
        unsigned int GetWidth();
        void SetWidth(unsigned int v);
        unsigned int GetHeight();
        void SetHeight(unsigned int v);
        Flood::PixelFormat GetPixelFormat();
        void SetPixelFormat(Flood::PixelFormat v);
        bool IsCompressed();
        Flood::ResourceGroup GetResourceGroup();
        void SetColor(Flood::Color color);
        void Log();
        void Create(unsigned int width, unsigned int height, Flood::PixelFormat format);
    };

    public ref class ImageWriter
    {
    public:
        property ::ImageWriter* NativePtr;

        ImageWriter(::ImageWriter* native);
        ImageWriter(System::IntPtr native);
        void Save(Flood::Image^ image, Flood::Stream^ stream);
        bool Convert(Flood::Image^ image);
    };
}
