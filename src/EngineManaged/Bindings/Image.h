/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Graphics/Resources/Image.h>
#include "Resource.h"
#include "ResourceHandle.h"

namespace Flood
{
    enum struct PixelFormat;
    enum struct ResourceGroup;
    ref class Allocator;
    ref class Enum;
    ref class Handle;
    ref class Image;
    ref class ImageWriter;
    ref class RefPtr;
    ref class Resource;
    value struct Color;
    value struct Vector2i;

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
    public ref class Image : Flood::Resource
    {
    public:
        Image(::Image* native);
        Image(System::IntPtr native);
        Image();
        Image(unsigned int width, unsigned int height, Flood::PixelFormat format);
        unsigned int GetWidth();
        void SetWidth(unsigned int v);
        unsigned int GetHeight();
        void SetHeight(unsigned int v);
        Flood::PixelFormat GetPixelFormat();
        void SetPixelFormat(Flood::PixelFormat v);
        System::Collections::Generic::List<unsigned char>^ GetBuffer();
        void SetBuffer(System::Collections::Generic::List<unsigned char>^ v);
        bool IsCompressed();
        void SetBuffer(System::IntPtr data, unsigned int size);
        void SetBuffer(Flood::Image^ image, Flood::Vector2i offset);
        unsigned int GetPixelSize();
        unsigned int GetSize();
        Flood::ResourceGroup GetResourceGroup();
        void SetColor(Flood::Color color);
        void Log();
        void Create(unsigned int width, unsigned int height, Flood::PixelFormat format);
        unsigned int GetTimestamp();
        static Flood::ResourceHandle<Flood::Image^> Create(Flood::Allocator^ _1, unsigned int width, unsigned int height, Flood::PixelFormat _2);
    };

    public ref class ImageWriter
    {
    public:
        property ::ImageWriter* NativePtr;

        ImageWriter(::ImageWriter* native);
        ImageWriter(System::IntPtr native);
        ImageWriter();
        bool Convert(Flood::Image^ image);
    };
}
