/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Resources/ResourceManager.h>
#include "ResourceHandle.h"

namespace Flood
{
    ref class ResourceManager;
    ref class Stream;
    ref class Archive;
    value struct FileWatchEvent;
    ref class ResourceTask;
    ref class ResourceLoader;
    ref class ResourceEvent;
    ref class Resource;
    ref class Handle;
    ref class Event1;
    ref class RefPtr;
    ref class Condition;
    ref class Mutex;
    value struct ResourceLoadOptions;
    ref class Allocator;

    /// <summary>
    /// Event fired whenever an operation on the resource happens.
    /// </summary>
    public ref class ResourceEvent
    {
    public:
        property ::ResourceEvent* NativePtr;

        ResourceEvent(::ResourceEvent* native);
        ResourceEvent(System::IntPtr native);
        ResourceEvent();
        property Flood::Resource^ Resource;
        property Flood::Resource^ OldResource;
        property Flood::ResourceHandle<Flood::Resource^> Handle;
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
    private:
        delegate void _ResourcePreparedDelegate(const ::ResourceEvent& _0);
        _ResourcePreparedDelegate^ _ResourcePreparedDelegateInstance;
        void _ResourcePreparedRaise(const ::ResourceEvent& _0);
        System::Action<Flood::ResourceEvent^>^ _ResourcePrepared;
    public:
        event System::Action<Flood::ResourceEvent^>^ ResourcePrepared
        {
            void add(System::Action<Flood::ResourceEvent^>^ evt);
            void remove(System::Action<Flood::ResourceEvent^>^ evt);
            void raise(Flood::ResourceEvent^);
        }
    private:
        delegate void _ResourceLoadedDelegate(const ::ResourceEvent& _0);
        _ResourceLoadedDelegate^ _ResourceLoadedDelegateInstance;
        void _ResourceLoadedRaise(const ::ResourceEvent& _0);
        System::Action<Flood::ResourceEvent^>^ _ResourceLoaded;
    public:
        event System::Action<Flood::ResourceEvent^>^ ResourceLoaded
        {
            void add(System::Action<Flood::ResourceEvent^>^ evt);
            void remove(System::Action<Flood::ResourceEvent^>^ evt);
            void raise(Flood::ResourceEvent^);
        }
    private:
        delegate void _ResourceRemovedDelegate(const ::ResourceEvent& _0);
        _ResourceRemovedDelegate^ _ResourceRemovedDelegateInstance;
        void _ResourceRemovedRaise(const ::ResourceEvent& _0);
        System::Action<Flood::ResourceEvent^>^ _ResourceRemoved;
    public:
        event System::Action<Flood::ResourceEvent^>^ ResourceRemoved
        {
            void add(System::Action<Flood::ResourceEvent^>^ evt);
            void remove(System::Action<Flood::ResourceEvent^>^ evt);
            void raise(Flood::ResourceEvent^);
        }
    private:
        delegate void _ResourceReloadedDelegate(const ::ResourceEvent& _0);
        _ResourceReloadedDelegate^ _ResourceReloadedDelegateInstance;
        void _ResourceReloadedRaise(const ::ResourceEvent& _0);
        System::Action<Flood::ResourceEvent^>^ _ResourceReloaded;
    public:
        event System::Action<Flood::ResourceEvent^>^ ResourceReloaded
        {
            void add(System::Action<Flood::ResourceEvent^>^ evt);
            void remove(System::Action<Flood::ResourceEvent^>^ evt);
            void raise(Flood::ResourceEvent^);
        }
    private:
        delegate void _ResourceLoaderRegisteredDelegate(const ::ResourceLoader& _0);
        _ResourceLoaderRegisteredDelegate^ _ResourceLoaderRegisteredDelegateInstance;
        void _ResourceLoaderRegisteredRaise(const ::ResourceLoader& _0);
        System::Action<Flood::ResourceLoader^>^ _ResourceLoaderRegistered;
    public:
        event System::Action<Flood::ResourceLoader^>^ ResourceLoaderRegistered
        {
            void add(System::Action<Flood::ResourceLoader^>^ evt);
            void remove(System::Action<Flood::ResourceLoader^>^ evt);
            void raise(Flood::ResourceLoader^);
        }
        Flood::ResourceHandle<Flood::Resource^> GetResource(System::String^ name);
        Flood::ResourceHandle<Flood::Resource^> LoadResource(System::String^ name);
        Flood::ResourceHandle<Flood::Resource^> LoadResource(Flood::ResourceLoadOptions options);
        bool FindResource(Flood::ResourceLoadOptions options);
        void RemoveResource(Flood::Resource^ resource);
        void RemoveResource(System::String^ name);
        void RemoveUnusedResources();
        void LoadQueuedResources();
        void Update();
        Flood::ResourceLoader^ FindLoader(System::String^ extension);
        bool GetAsynchronousLoading();
        void SetAsynchronousLoading(bool v);
    };

    public ref class FloodResourceManager
    {
    public:
        static Flood::ResourceManager^ GetResourceManager();
    };
}
