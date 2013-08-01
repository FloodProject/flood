/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "CppSharp.h"
#include <Resources/ResourceLoader.h>
#include "Extension.h"

namespace Flood
{
    enum struct ResourceGroup;
    ref class Resource;
    ref class ResourceLoader;
    ref class ResourceStream;
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
        Flood::Resource^ Resource;
        Flood::ResourceGroup Group;
        bool IsHighPriority;
        bool SendLoadEvent;
        bool AsynchronousLoad;
        bool KeepStreamOpen;
        Flood::ResourceLoadOption Option;
        void AddOption(int key, int value);
    };

    public ref class ResourceStream : ICppInstance
    {
    public:
        property ::ResourceStream* NativePtr;
        property System::IntPtr Instance
        {
            virtual System::IntPtr get();
            virtual void set(System::IntPtr instance);
        }

        ResourceStream(::ResourceStream* native);
        ResourceStream(System::IntPtr native);
        ResourceStream();
        property Flood::ResourceLoader^ Loader
        {
            Flood::ResourceLoader^ get();
            void set(Flood::ResourceLoader^);
        }
        int Decode(System::IntPtr buffer, unsigned int size);
        void Reset();
        virtual bool Equals(System::Object^ object) override;
        virtual int GetHashCode() override;
    };

    public ref class ResourceLoader : Flood::Extension
    {
    public:
        ResourceLoader(::ResourceLoader* native);
        ResourceLoader(System::IntPtr native);
        ResourceLoader();
        property Flood::ExtensionMetadata Metadata
        {
            Flood::ExtensionMetadata get();
        }
        property System::String^ Name
        {
            System::String^ get();
        }
        property Flood::ResourceGroup ResourceGroup
        {
            Flood::ResourceGroup get();
        }
        property System::Collections::Generic::List<System::String^>^ Extensions
        {
            System::Collections::Generic::List<System::String^>^ get();
        }
        property System::Collections::Generic::List<System::String^>^ Extensions1
        {
            System::Collections::Generic::List<System::String^>^ get();
            void set(System::Collections::Generic::List<System::String^>^);
        }
        Flood::Resource^ Prepare(Flood::ResourceLoadOptions _0);
        bool Decode(Flood::ResourceLoadOptions _0);
        virtual bool Equals(System::Object^ object) override;
        virtual int GetHashCode() override;
    };
}
