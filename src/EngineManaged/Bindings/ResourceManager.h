/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "CppSharp.h"
#include <Resources/ResourceManager.h>
#include "Resource.h"
#include "ResourceHandle.h"

namespace Flood
{
    ref class Archive;
    ref class ResourceEvent;
    ref class ResourceLoader;
    ref class ResourceManager;
    ref class Stream;
    value struct FileWatchEvent;
    value struct ResourceLoadOptions;
}

namespace Flood
{
    /// <summary>
    /// Event fired whenever an operation on the resource happens. This can be
    /// useful to know when monitoring for changes, for example in editors.
    /// </summary>
    public ref class ResourceEvent : ICppInstance
    {
    public:

        property ::ResourceEvent* NativePtr;
        property System::IntPtr __Instance
        {
            virtual System::IntPtr get();
            virtual void set(System::IntPtr instance);
        }

        ResourceEvent(::ResourceEvent* native);
        ResourceEvent(System::IntPtr native);
        ResourceEvent();

        property Flood::Resource^ Resource
        {
            Flood::Resource^ get();
            void set(Flood::Resource^);
        }

        property Flood::Resource^ OldResource
        {
            Flood::Resource^ get();
            void set(Flood::Resource^);
        }

        property Flood::ResourceHandle<Flood::Resource^> Handle
        {
            Flood::ResourceHandle<Flood::Resource^> get();
            void set(Flood::ResourceHandle<Flood::Resource^>);
        }

        virtual bool Equals(System::Object^ object) override;

        virtual int GetHashCode() override;
    };

    /// <summary>
    /// Responsible for managing a set of resources that are added by the app. It
    /// should be possible to enforce a strict memory budget, and the manager will
    /// automatically load or unload the resources from memory, using for example
    /// an LRU policy.
    /// </summary>
    public ref class ResourceManager : ICppInstance
    {
    public:

        property ::ResourceManager* NativePtr;
        property System::IntPtr __Instance
        {
            virtual System::IntPtr get();
            virtual void set(System::IntPtr instance);
        }

        ResourceManager(::ResourceManager* native);
        ResourceManager(System::IntPtr native);
        ResourceManager();

        property bool AsynchronousLoading
        {
            bool get();
            void set(bool);
        }

        property Flood::Archive^ Archive
        {
            Flood::Archive^ get();
            void set(Flood::Archive^);
        }

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
            void raise(Flood::ResourceEvent^ _0);
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
            void raise(Flood::ResourceEvent^ _0);
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
            void raise(Flood::ResourceEvent^ _0);
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
            void raise(Flood::ResourceEvent^ _0);
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
            void raise(Flood::ResourceLoader^ _0);
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

        virtual bool Equals(System::Object^ object) override;

        virtual int GetHashCode() override;

        static Flood::ResourceManager^ GetResourceManager();

        generic<typename T> where T : Flood::Resource
        Flood::ResourceHandle<T> GetResource(System::String^ name);

        generic<typename T> where T : Flood::Resource
        Flood::ResourceHandle<T> LoadResource(System::String^ name);

        generic<typename T> where T : Flood::Resource
        Flood::ResourceHandle<T> LoadResource(Flood::ResourceLoadOptions options);
    };
}
