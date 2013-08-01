/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "CppSharp.h"
#include <Resources/Resource.h>
#include "ResourceHandle.h"

namespace Flood
{
    enum struct ResourceGroup;
    enum struct ResourceStatus;
    ref class ResourceStream;

    public enum struct ResourceStatus
    {
        Error = 0,
        Unloaded = 1,
        Loading = 2,
        Loaded = 3
    };

    public enum struct ResourceGroup
    {
        General = 0,
        Images = 1,
        Meshes = 2,
        Fonts = 3,
        Shaders = 4,
        Audio = 5,
        Scripts = 6,
        Scenes = 7,
        Materials = 8,
        Particles = 9
    };

    public ref class Resource : ICppInstance
    {
    public:
        property ::Resource* NativePtr;
        property System::IntPtr Instance
        {
            virtual System::IntPtr get();
            virtual void set(System::IntPtr instance);
        }

        Resource(::Resource* native);
        Resource(System::IntPtr native);
        property System::String^ Path
        {
            System::String^ get();
            void set(System::String^);
        }
        property Flood::ResourceStatus Status
        {
            Flood::ResourceStatus get();
            void set(Flood::ResourceStatus);
        }
        property Flood::ResourceGroup ResourceGroup
        {
            Flood::ResourceGroup get();
        }
        property System::String^ Path1
        {
            System::String^ get();
            void set(System::String^);
        }
        property Flood::ResourceStatus Status1
        {
            Flood::ResourceStatus get();
            void set(Flood::ResourceStatus);
        }
        property Flood::ResourceStream^ Stream
        {
            Flood::ResourceStream^ get();
            void set(Flood::ResourceStream^);
        }
        bool IsLoaded();
        virtual bool Equals(System::Object^ object) override;
        virtual int GetHashCode() override;
        Flood::ResourceHandle<Flood::Resource^> HandleCreate();
    };
}
