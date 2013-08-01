/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "CppSharp.h"
#include <Graphics/Resources/Image.h>
#include "Resource.h"
#include "ResourceHandle.h"

namespace Flood
{
    enum struct PixelFormat;
    enum struct ResourceGroup;
    ref class Allocator;
    ref class ImageWriter;
    value struct Color;
    value struct Vector2i;

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

    public ref class Image : Flood::Resource
    {
    public:
        Image(::Image* native);
        Image(System::IntPtr native);
        Image();
        Image(unsigned int width, unsigned int height, Flood::PixelFormat format);
        property unsigned int Width
        {
            unsigned int get();
            void set(unsigned int);
        }
        property unsigned int Height
        {
            unsigned int get();
            void set(unsigned int);
        }
        property Flood::PixelFormat PixelFormat
        {
            Flood::PixelFormat get();
            void set(Flood::PixelFormat);
        }
        property System::Collections::Generic::List<unsigned char>^ Buffer
        {
            System::Collections::Generic::List<unsigned char>^ get();
        }
        property System::Collections::Generic::List<unsigned char>^ Buffer1
        {
            System::Collections::Generic::List<unsigned char>^ get();
        }
        property unsigned int PixelSize
        {
            unsigned int get();
        }
        property unsigned int Size
        {
            unsigned int get();
        }
        property Flood::ResourceGroup ResourceGroup
        {
            Flood::ResourceGroup get();
        }
        property Flood::Color Color
        {
            void set(Flood::Color);
        }
        property unsigned int Timestamp
        {
            unsigned int get();
        }
        void SetBuffer(System::Collections::Generic::List<unsigned char>^ v);
        bool IsCompressed();
        void SetBuffer(System::IntPtr data);
        void SetBuffer(System::IntPtr data, unsigned int stride);
        void SetBuffer(Flood::Image^ image, Flood::Vector2i offset);
        void Log();
        void Create(unsigned int width, unsigned int height, Flood::PixelFormat format);
        void SetModified();
        virtual bool Equals(System::Object^ object) override;
        virtual int GetHashCode() override;
        static Flood::ResourceHandle<Flood::Image^> Create(Flood::Allocator^ _0, unsigned int width, unsigned int height, Flood::PixelFormat _1);
    };

    public ref class ImageWriter : ICppInstance
    {
    public:
        property ::ImageWriter* NativePtr;
        property System::IntPtr Instance
        {
            virtual System::IntPtr get();
            virtual void set(System::IntPtr instance);
        }

        ImageWriter(::ImageWriter* native);
        ImageWriter(System::IntPtr native);
        ImageWriter();
        bool Convert(Flood::Image^ image);
        virtual bool Equals(System::Object^ object) override;
        virtual int GetHashCode() override;
    };
}
