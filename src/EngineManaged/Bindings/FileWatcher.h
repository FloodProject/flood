/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "CppSharp.h"
#include <Core/FileWatcher.h>

namespace Flood
{
    enum struct FileWatchEventKind;
    ref class FileWatcher;
    value struct FileWatchEvent;
}

namespace Flood
{
    public enum struct FileWatchEventKind
    {
        Added = 0,
        Deleted = 1,
        Modified = 2,
        Renamed = 3
    };

    public value struct FileWatchEvent
    {
    public:

        FileWatchEvent(::FileWatchEvent* native);
        FileWatchEvent(System::IntPtr native);
        FileWatchEvent(Flood::FileWatchEventKind _0, unsigned int _1, System::String^ dir, System::String^ file);

        property Flood::FileWatchEventKind Action
        {
            Flood::FileWatchEventKind get();
            void set(Flood::FileWatchEventKind);
        }

        property unsigned int WatchId
        {
            unsigned int get();
            void set(unsigned int);
        }

        property System::String^ Dir
        {
            System::String^ get();
            void set(System::String^);
        }

        property System::String^ Filename
        {
            System::String^ get();
            void set(System::String^);
        }

        property void* Userdata
        {
            void* get();
            void set(void*);
        }

        private:
        Flood::FileWatchEventKind __Action;
        unsigned int __WatchId;
        System::String^ __Dir;
        System::String^ __Filename;
        void* __Userdata;
    };

    /// <summary>
    /// Listens to files and directories and dispatches events to notify the parent
    /// program of the changes.
    /// </summary>
    public ref class FileWatcher : ICppInstance
    {
    public:

        property ::FileWatcher* NativePtr;
        property System::IntPtr __Instance
        {
            virtual System::IntPtr get();
            virtual void set(System::IntPtr instance);
        }

        FileWatcher(::FileWatcher* native);
        FileWatcher(System::IntPtr native);
        FileWatcher();

    private:
        delegate void _FileWatchEventDelegate(const ::FileWatchEvent& _0);
        _FileWatchEventDelegate^ _FileWatchEventDelegateInstance;
        void _FileWatchEventRaise(const ::FileWatchEvent& _0);
        System::Action<Flood::FileWatchEvent>^ _FileWatchEvent;
    public:
        event System::Action<Flood::FileWatchEvent>^ FileWatchEvent
        {
            void add(System::Action<Flood::FileWatchEvent>^ evt);
            void remove(System::Action<Flood::FileWatchEvent>^ evt);
            void raise(Flood::FileWatchEvent _0);
        }
        virtual unsigned int AddWatch(System::String^ directory, void* userdata);

        virtual void RemoveWatch(System::String^ directory);

        virtual void RemoveWatch(unsigned int FileWatchId);

        virtual void Update();

        virtual bool Equals(System::Object^ object) override;

        virtual int GetHashCode() override;
    };
}
