/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Resources/ResourceManager.h>

namespace Flood
{
    ref class ResourceManager;
    ref class Archive;
    value struct FileWatchEvent;
    value struct ResourceEvent;
    ref class ResourceTask;
    ref class Event1;
    ref class ConcurrentQueue;
    ref class TaskPool;
    ref class HandleManager;
    ref class Condition;
    ref class Mutex;
    ref class Handle;
    value struct ResourceLoadOptions;
    ref class Resource;
    ref class ResourceLoader;
    ref class Class;
    ref class RefPtr;





    /// <summary>
    /// Responsible for managing a set of resources that are added by the app.
    /// It should be possible to enforce a strict memory budget, and the manager
    /// will automatically load or unload the resources from memory, using for
    /// example an LRU policy.
    /// </summary>
    public ref class ResourceManager
    {
        property ::ResourceManager* NativePtr;

    public:
        ResourceManager(::ResourceManager* native);
        ResourceManager();
        uint GetResource(System::String^ name);
        uint LoadResource(System::String^ name);
        uint LoadResource(Flood::ResourceLoadOptions options);
        bool FindResource(Flood::ResourceLoadOptions options);
        void RemoveResource(Flood::Resource^ resource);
        void RemoveResource(System::String^ name);
        void RemoveUnusedResources();
        void LoadQueuedResources();
        void Update();
        System::Collections::Generic::Dictionary<System::String^, uint>^ GetResources();
        Flood::ResourceLoader^ FindLoader(System::String^ extension);
        Flood::ResourceLoader^ FindLoaderByClass(Flood::Class^ klass);
        void SetupResourceLoaders(Flood::Class^ klass);
        System::Collections::Generic::Dictionary<System::String^, Flood::ResourceLoader^>^ GetResourceLoaders();
        bool GetAsynchronousLoading();
        void SetAsynchronousLoading(bool v);
        Flood::HandleManager^ GetHandleManager();
        Flood::TaskPool^ GetTaskPool();
        void SetTaskPool(Flood::TaskPool^ v);
        Flood::Archive^ GetArchive();
        void SetArchive(Flood::Archive^ archive);
    };



    /// <summary>
    /// Event fired whenever an operation on the resource happens. This can be
    /// useful to know when monitoring for changes, for example in editors.
    /// </summary>
    public value struct ResourceEvent
    {
    };


    public ref class FloodResourceManager
    {
    public:
        static Flood::ResourceManager^ GetResourceManager ();
    };
}

