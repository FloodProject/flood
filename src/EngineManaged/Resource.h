/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Resources/Resource.h>

namespace Flood
{
    enum struct ResourceStatus;
    enum struct ResourceGroup;
    ref class Enum;
    ref class Class;
    ref class ReferenceCounted;
    ref class Handle;
    ref class Resource;
    ref class ResourceStream;

    /// <summary>
    /// Resources can be loaded in a background task. In that case the caller
    /// will still receive a resource but it won't be fully loaded. It will only
    /// be fully loaded when the resource status changes to loaded.
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
    /// manager class uses this information to keep the related resources
    /// together, and for managing the data.
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
    /// data that is used by the various systems of the engine, for example
    /// mesh, sounds, fonts, images, shaders, and other types of resource data.
    /// Each resource is identified by a path (Uniform Resource Identifier),
    /// that way we can add virtual resources (it could be used for various
    /// things, like procedural content generation and to identify streaming
    /// resource from a network connection).
    /// </summary>
    public ref class Resource
    {
        property ::Resource* NativePtr;

    public:
        Resource(::Resource* native);
        Flood::Class^ GetType();
        Flood::Class^ GetStaticType();
        System::String^ GetPath();
        void SetPath(System::String^ v);
        Flood::ResourceStatus GetStatus();
        void SetStatus(Flood::ResourceStatus v);
        bool IsLoaded();
        Flood::ResourceGroup GetResourceGroup();
    };

    public ref class FloodResource
    {
    public:
        static Flood::Enum^ ResourceStatusGetType ();
        static Flood::Enum^ ResourceGroupGetType ();
        static Flood::Class^ ResourceGetType ();
        static Flood::ReferenceCounted^ ResourceHandleFind (unsigned int id);
        static void ResourceHandleDestroy (unsigned int id);
        static uint ResourceHandleCreate (Flood::Resource^ _129);
    };
}

