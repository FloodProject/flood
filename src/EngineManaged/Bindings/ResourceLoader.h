/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Resources/ResourceLoader.h>
#include "Extension.h"
#include "Resource.h"
#include "ResourceHandle.h"

namespace Flood
{
    enum struct ResourceGroup;
    ref class Extension;
    ref class RefPtr;
    ref class Resource;
    ref class ResourceLoader;
    ref class ResourceStream;
    ref class Stream;
    value struct ExtensionMetadata;
    value struct ResourceLoadOption;
    value struct ResourceLoadOptions;

    public value struct ResourceLoadOption
    {
    public:
        ResourceLoadOption(::ResourceLoadOption* native);
        ResourceLoadOption(System::IntPtr native);
        int Key;
        int Value;
    };

    public value struct ResourceLoadOptions
    {
    public:
        ResourceLoadOptions(::ResourceLoadOptions* native);
        ResourceLoadOptions(System::IntPtr native);
        System::String^ Name;
        Flood::Stream^ Stream;
        Flood::Resource^ Resource;
        Flood::ResourceGroup Group;
        bool IsHighPriority;
        bool SendLoadEvent;
        bool AsynchronousLoad;
        bool KeepStreamOpen;
        Flood::ResourceLoadOption Option;
        void AddOption(int key, int value);
    };

    public ref class ResourceStream
    {
    public:
        property ::ResourceStream* NativePtr;

        ResourceStream(::ResourceStream* native);
        ResourceStream(System::IntPtr native);
        ResourceStream();
        property Flood::Stream^ Stream
        {
            Flood::Stream^ get();
            void set(Flood::Stream^);
        }
        property Flood::ResourceLoader^ Loader
        {
            Flood::ResourceLoader^ get();
            void set(Flood::ResourceLoader^);
        }
        int Decode(System::IntPtr buffer, unsigned int size);
        void Reset();
    };

    /// <summary>
    /// Resource loaders decode resource data into a proper resource. This is an
    /// interface that should be implemented to provide the resource codec services
    /// for the formats handled by the loader. The loader also provides the
    /// extensions of those formats.
    /// </summary>
    public ref class ResourceLoader : Flood::Extension
    {
    public:
        ResourceLoader(::ResourceLoader* native);
        ResourceLoader(System::IntPtr native);
        ResourceLoader();
        property System::Collections::Generic::List<System::String^>^ Extensions
        {
            System::Collections::Generic::List<System::String^>^ get();
            void set(System::Collections::Generic::List<System::String^>^);
        }
        Flood::ExtensionMetadata GetMetadata();
        Flood::Resource^ Prepare(Flood::ResourceLoadOptions _0);
        bool Decode(Flood::ResourceLoadOptions _0);
        System::String^ GetName();
        Flood::ResourceGroup GetResourceGroup();
        System::Collections::Generic::List<System::String^>^ GetExtensions();
    };
}
