/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Core/Concurrency.h>

namespace Flood
{
    enum struct ThreadPriority;
    enum struct TaskState;
    ref class Thread;
    ref class Allocator;
    ref class Delegate2;
    ref class Mutex;
    ref class Condition;
    ref class Task;
    ref class TaskPool;
    ref class TaskEvent;
    ref class Delegate1;
    ref class ConcurrentQueue;
    ref class Event1;
    ref class scoped_ptr;

    /// <summary>
    /// A thread is the entity within a process that can be scheduled for
    /// execution. All threads of a process share its virtual address space and
    /// system resources.
    /// </summary>
    public enum struct ThreadPriority
    {
        Low = 0,
        Normal = 1,
        High = 2
    };

    public enum struct TaskState
    {
        Added = 0,
        Started = 1,
        Finished = 2
    };









    public ref class Thread
    {
        property ::Thread* NativePtr;

    public:
        Thread(::Thread* native);
    };



    /// <summary>
    /// Tasks provide an higher level interface to concurrency than threads.
    /// They can be managed by the engine and grouped in different hardware
    /// threads.
    /// </summary>
    public ref class Task
    {
        property ::Task* NativePtr;

    public:
        Task(::Task* native);
    };

    public ref class TaskEvent
    {
        property ::TaskEvent* NativePtr;

    public:
        TaskEvent(::TaskEvent* native);
    };

    public ref class TaskPool
    {
        property ::TaskPool* NativePtr;

    public:
        TaskPool(::TaskPool* native);
    };

    public ref class FloodConcurrency
    {
    public:
        static Flood::Thread^ ThreadCreate (Flood::Allocator^ _10);
        static void ThreadDestroy (Flood::Thread^ _11);
        static bool ThreadStart (Flood::Thread^ _12, Delegate2 _13, System::IntPtr _14);
        static bool ThreadJoin (Flood::Thread^ _15);
        static bool ThreadPause (Flood::Thread^ _16);
        static bool ThreadResume (Flood::Thread^ _17);
        static bool ThreadSetPriority (Flood::Thread^ _18, Flood::ThreadPriority _19);
        static void ThreadSetName (Flood::Thread^ _20, System::String^ name);
        static Flood::Mutex^ MutexCreate (Flood::Allocator^ _21);
        static void MutexDestroy (Flood::Mutex^ _22);
        static void MutexInit (Flood::Mutex^ _23);
        static void MutexLock (Flood::Mutex^ _24);
        static void MutexUnlock (Flood::Mutex^ _25);
        static Flood::Condition^ ConditionCreate (Flood::Allocator^ _26);
        static void ConditionDestroy (Flood::Condition^ _27);
        static void ConditionInit (Flood::Condition^ _28);
        static void ConditionWait (Flood::Condition^ _29, Flood::Mutex^ _30);
        static void ConditionWakeOne (Flood::Condition^ _31);
        static void ConditionWakeAll (Flood::Condition^ _32);
        static int AtomicRead (int atomic);
        static int AtomicWrite (int atomic, int value);
        static int AtomicAdd (int atomic, int value);
        static int AtomicIncrement (int atomic);
        static int AtomicDecrement (int atomic);
        static Flood::Task^ TaskCreate (Flood::Allocator^ _33);
        static void TaskDestroy (Flood::Task^ _34);
        static void TaskRun (Flood::Task^ _35);
        static Flood::TaskPool^ TaskPoolCreate (Flood::Allocator^ _36, char Size);
        static void TaskPoolDestroy (Flood::TaskPool^ _37);
        static void TaskPoolAdd (Flood::TaskPool^ _38, Flood::Task^ _39, unsigned char Priority);
        static void TaskPoolUpdate (Flood::TaskPool^ _40);
    };
}

