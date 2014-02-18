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
    ref class Stream;
    value struct ExtensionMetadata;
    value struct ResourceLoadOption;
    value struct ResourceLoadOptions;
}

namespace Flood
{
    public value struct ResourceLoadOption
    {
    public:

        ResourceLoadOption(::ResourceLoadOption* native);
        ResourceLoadOption(System::IntPtr native);
        property int Key
        {
            int get();
            void set(int);
        }

        property int Value
        {
            int get();
            void set(int);
        }

        private:
        int __Key;
        int __Value;
    };

    public value struct ResourceLoadOptions
    {
    public:

        ResourceLoadOptions(::ResourceLoadOptions* native);
        ResourceLoadOptions(System::IntPtr native);
        property System::String^ Name
        {
            System::String^ get();
            void set(System::String^);
        }

        property Flood::Stream^ Stream
        {
            Flood::Stream^ get();
            void set(Flood::Stream^);
        }

        property Flood::Resource^ Resource
        {
            Flood::Resource^ get();
            void set(Flood::Resource^);
        }

        property Flood::ResourceGroup Group
        {
            Flood::ResourceGroup get();
            void set(Flood::ResourceGroup);
        }

        property bool IsHighPriority
        {
            bool get();
            void set(bool);
        }

        property bool SendLoadEvent
        {
            bool get();
            void set(bool);
        }

        property bool AsynchronousLoad
        {
            bool get();
            void set(bool);
        }

        property bool KeepStreamOpen
        {
            bool get();
            void set(bool);
        }

        Flood::ResourceLoadOption Option;
        void AddOption(int key, int value);

        private:
        System::String^ __Name;
        Flood::Stream^ __Stream;
        Flood::Resource^ __Resource;
        Flood::ResourceGroup __Group;
        bool __IsHighPriority;
        bool __SendLoadEvent;
        bool __AsynchronousLoad;
        bool __KeepStreamOpen;
    };

    public ref class ResourceStream : ICppInstance
    {
    public:

        property ::ResourceStream* NativePtr;
        property System::IntPtr __Instance
        {
            virtual System::IntPtr get();
            virtual void set(System::IntPtr instance);
        }

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

        virtual int Decode(System::IntPtr buffer, unsigned int size);

        virtual void Reset();

        virtual bool Equals(System::Object^ object) override;

        virtual int GetHashCode() override;
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

        virtual Flood::Resource^ Prepare(Flood::ResourceLoadOptions _0);

        virtual bool Decode(Flood::ResourceLoadOptions _1);

        virtual bool Equals(System::Object^ object) override;

        virtual int GetHashCode() override;
    };
}
