/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Concurrency.h"
#include "Memory.h"

using namespace System;
using namespace System::Runtime::InteropServices;

Flood::Thread::Thread(::Thread* native)
{
    NativePtr = native;
}

Flood::Task::Task(::Task* native)
{
    NativePtr = native;
}

Flood::TaskEvent::TaskEvent(::TaskEvent* native)
{
    NativePtr = native;
}

Flood::TaskPool::TaskPool(::TaskPool* native)
{
    NativePtr = native;
}

Flood::Thread^ Flood::FloodConcurrency::ThreadCreate(Flood::Allocator^ _10)
{
    return nullptr;
}

void Flood::FloodConcurrency::ThreadDestroy(Flood::Thread^ _11)
{
}

bool Flood::FloodConcurrency::ThreadStart(Flood::Thread^ _12, Delegate2 _13, System::IntPtr _14)
{
    return false;
}

bool Flood::FloodConcurrency::ThreadJoin(Flood::Thread^ _15)
{
    return false;
}

bool Flood::FloodConcurrency::ThreadPause(Flood::Thread^ _16)
{
    return false;
}

bool Flood::FloodConcurrency::ThreadResume(Flood::Thread^ _17)
{
    return false;
}

bool Flood::FloodConcurrency::ThreadSetPriority(Flood::Thread^ _18, Flood::ThreadPriority _19)
{
    return false;
}

void Flood::FloodConcurrency::ThreadSetName(Flood::Thread^ _20, System::String^ name)
{
}

Flood::Mutex^ Flood::FloodConcurrency::MutexCreate(Flood::Allocator^ _21)
{
    return nullptr;
}

void Flood::FloodConcurrency::MutexDestroy(Flood::Mutex^ _22)
{
}

void Flood::FloodConcurrency::MutexInit(Flood::Mutex^ _23)
{
}

void Flood::FloodConcurrency::MutexLock(Flood::Mutex^ _24)
{
}

void Flood::FloodConcurrency::MutexUnlock(Flood::Mutex^ _25)
{
}

Flood::Condition^ Flood::FloodConcurrency::ConditionCreate(Flood::Allocator^ _26)
{
    return nullptr;
}

void Flood::FloodConcurrency::ConditionDestroy(Flood::Condition^ _27)
{
}

void Flood::FloodConcurrency::ConditionInit(Flood::Condition^ _28)
{
}

void Flood::FloodConcurrency::ConditionWait(Flood::Condition^ _29, Flood::Mutex^ _30)
{
}

void Flood::FloodConcurrency::ConditionWakeOne(Flood::Condition^ _31)
{
}

void Flood::FloodConcurrency::ConditionWakeAll(Flood::Condition^ _32)
{
}

int Flood::FloodConcurrency::AtomicRead(int atomic)
{
    return 0;
}

int Flood::FloodConcurrency::AtomicWrite(int atomic, int value)
{
    return 0;
}

int Flood::FloodConcurrency::AtomicAdd(int atomic, int value)
{
    return 0;
}

int Flood::FloodConcurrency::AtomicIncrement(int atomic)
{
    return 0;
}

int Flood::FloodConcurrency::AtomicDecrement(int atomic)
{
    return 0;
}

Flood::Task^ Flood::FloodConcurrency::TaskCreate(Flood::Allocator^ _33)
{
    return nullptr;
}

void Flood::FloodConcurrency::TaskDestroy(Flood::Task^ _34)
{
}

void Flood::FloodConcurrency::TaskRun(Flood::Task^ _35)
{
}

Flood::TaskPool^ Flood::FloodConcurrency::TaskPoolCreate(Flood::Allocator^ _36, char Size)
{
    return nullptr;
}

void Flood::FloodConcurrency::TaskPoolDestroy(Flood::TaskPool^ _37)
{
}

void Flood::FloodConcurrency::TaskPoolAdd(Flood::TaskPool^ _38, Flood::Task^ _39, unsigned char Priority)
{
}

void Flood::FloodConcurrency::TaskPoolUpdate(Flood::TaskPool^ _40)
{
}


