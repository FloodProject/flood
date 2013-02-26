/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Resources/ResourceManager.h>
#include "Stream.h"
#include "Memory.h"
#include "Reflection.h"
#include "ResourceLoader.h"
#include "Extension.h"

namespace Flood
{
    ref class ResourceManager;
    ref class Event1;
    ref class Delegate0;
    ref class ClosurePtr;
    ref class GenericClass;
    ref class DelegateMemento;
    ref class ConcurrentQueue;
    ref class Mutex;
    ref class Condition;
    ref class Archive;
    ref class TaskPool;
    ref class HandleManager;
    ref class Handle;
    value struct ResourceLoadOptions;
    ref class Stream;
    enum struct StreamOpenMode;
    ref class StreamFuncs;
    ref class Allocator;
    enum struct StreamSeekMode;
    ref class MemoryStream;
    ref class Resource;
    enum struct ResourceStatus;
    ref class ResourceStream;
    ref class Class;
    ref class Field;
    ref class Type;
    enum struct TypeKind : unsigned char;
    ref class ReflectionContext;
    enum struct ReflectionWalkType : unsigned char;
    enum struct FieldQualifier : unsigned short;
    enum struct ResourceGroup;
    ref class ResourceLoader;
    value struct ExtensionMetadata;
    value struct FileWatchEvent;
    ref class ResourceTask;
    value struct ResourceEvent;
    ref class RefPtr;

    /// <summary>
    /// Event fired whenever an operation on the resource happens. This can be
    /// useful to know when monitoring for changes, for example in editors.
    /// </summary>
    public value struct ResourceEvent
    {
    public:
        ResourceEvent(::ResourceEvent* native);
        ResourceEvent(System::IntPtr native);
        Flood::Resource^ Resource;
        Flood::Resource^ OldResource;
        uint Handle;
    };

    /// <summary>
    /// Responsible for managing a set of resources that are added by the app. It
    /// should be possible to enforce a strict memory budget, and the manager will
    /// automatically load or unload the resources from memory, using for example
    /// an LRU policy.
    /// </summary>
    public ref class ResourceManager
    {
    public:
        property ::ResourceManager* NativePtr;

        ResourceManager(::ResourceManager* native);
        ResourceManager(System::IntPtr native);
        ResourceManager();
        property bool AsynchronousLoading;
        property int NumResourcesQueuedLoad;
        uint GetResource(System::String^ name);
        uint LoadResource(System::String^ name);
        uint LoadResource(Flood::ResourceLoadOptions options);
        bool FindResource(Flood::ResourceLoadOptions options);
        void RemoveResource(Flood::Resource^ resource);
        void RemoveResource(System::String^ name);
        void RemoveUnusedResources();
        void LoadQueuedResources();
        void Update();
        Flood::ResourceLoader^ FindLoader(System::String^ extension);
        Flood::ResourceLoader^ FindLoaderByClass(Flood::Class^ klass);
        void SetupResourceLoaders(Flood::Class^ klass);
        bool GetAsynchronousLoading();
        void SetAsynchronousLoading(bool v);
    };

    public ref class FloodResourceManager
    {
    public:
        static Flood::ResourceManager^ GetResourceManager();
    };
}
