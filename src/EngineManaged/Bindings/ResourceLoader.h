/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Resources/ResourceLoader.h>

namespace Flood
{
    ref class Class;
    ref class ResourceStream;
    ref class ResourceLoader;
    value struct ResourceLoadOptions;
    value struct ResourceLoadOption;
    ref class Stream;
    value struct ExtensionMetadata;
    ref class Resource;
    enum struct ResourceGroup;
    ref class RefPtr;


    public ref class ResourceStream
    {
        property ::ResourceStream* NativePtr;

    public:
        ResourceStream(::ResourceStream* native);
        ResourceStream();
        int Decode(unsigned char buffer, unsigned int size);
        void Reset();
    };

    /// <summary>
    /// Resource loaders decode resource data into a proper resource. This is an
    /// interface that should be implemented to provide the resource codec
    /// services for the formats handled by the loader. The loader also provides
    /// the extensions of those formats.
    /// </summary>
    public ref class ResourceLoader
    {
        property ::ResourceLoader* NativePtr;

    public:
        ResourceLoader(::ResourceLoader* native);
        ResourceLoader();
        Flood::Class^ GetType();
        Flood::Class^ GetStaticType();
        Flood::ExtensionMetadata GetMetadata();
        Flood::Resource^ Prepare(Flood::ResourceLoadOptions);
        bool Decode(Flood::ResourceLoadOptions);
        System::String^ GetName();
        Flood::Class^ GetResourceClass();
        Flood::ResourceGroup GetResourceGroup();
        System::Collections::Generic::List<System::String^>^ GetExtensions();
    };

    public value struct ResourceLoadOptions
    {
        void AddOption(int key, int value);
    };

    public value struct ResourceLoadOption
    {
    };

    public ref class FloodResourceLoader
    {
    public:
        static Flood::Class^ ResourceLoaderGetType ();
    };
}

