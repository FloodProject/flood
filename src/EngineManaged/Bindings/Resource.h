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
#include "StringConverter.h"

namespace Flood
{
    enum struct ResourceGroup;
    enum struct ResourceStatus;
    ref class ResourceStream;
}

namespace Flood
{
    /// <summary>
    /// Resources can be loaded in a background task. In that case the caller will
    /// still receive a resource but it won't be fully loaded. It will only be
    /// fully loaded when the resource status changes to loaded.
    /// </summary>
    public enum struct ResourceStatus
    {
        Error = 0,
        Unloaded = 1,
        Loading = 2,
        Loaded = 3
    };

    /// <summary>
    /// Resource groups identify the kind of data a resource holds. The resource
    /// manager class uses this information to keep the related resources together,
    /// and for managing the data.
    /// </summary>
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

    /// <summary>
    /// Represents a generic resource that tipically is used to hold a piece of
    /// data that is used by the various systems of the engine, for example mesh,
    /// sounds, fonts, images, shaders, and other types of resource data. Each
    /// resource is identified by a path (Uniform Resource Identifier), that way we
    /// can add virtual resources (it could be used for various things, like
    /// procedural content generation and to identify streaming resource from a
    /// network connection).
    /// </summary>
    public ref class Resource : ICppInstance
    {
    public:

        property ::Resource* NativePtr;
        property System::IntPtr __Instance
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

        /// <summary>
        /// Gets if the resource is fully loaded.
        /// </summary>
        bool IsLoaded();

        virtual bool Equals(System::Object^ object) override;

        virtual int GetHashCode() override;

        Flood::ResourceHandle<Flood::Resource^> HandleCreate();
    };
}
