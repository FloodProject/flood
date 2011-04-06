/************************************************************************
*
* vapor3D Engine © (2008-2011)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/API.h"
#include "Core/Event.h"
#include "Core/Pointers.h"
#include "Core/ConcurrentQueue.h"

#ifdef VAPOR_PLATFORM_WINDOWS
	#define WIN32_LEAN_AND_MEAN
	#define NOMINMAX
	#include <Windows.h>
#endif

NAMESPACE_EXTERN_BEGIN

//-----------------------------------//

/**
 * A thread is the entity within a process that can be scheduled for
 * execution. All threads of a process share its virtual address space
 * and system resources.
 */

struct ThreadPriority
{
	enum Enum { Low = -1, Normal, High };
};

typedef void (*ThreadFunction)(void*);

struct Thread
{
	void* Handle;
	volatile bool IsRunning;
	ThreadPriority::Enum Priority;
	ThreadFunction Function;
	void* Userdata;
};

API_CORE Thread* ThreadCreate(Allocator*);
API_CORE void    ThreadDestroy(Thread*, Allocator*);
API_CORE bool    ThreadStart(Thread*, ThreadFunction, void*);
API_CORE bool    ThreadJoin(Thread*);
API_CORE bool    ThreadPause(Thread*);
API_CORE bool    ThreadResume(Thread*);
API_CORE bool    ThreadSetPriority(Thread*, ThreadPriority::Enum);
API_CORE void    ThreadSetName(Thread*, const String& name);

typedef scoped_ptr<Thread, ThreadDestroy> ThreadPtr;
#define pThreadCreate(alloc, ...) CreateScopedPtr(ThreadCreate, alloc, __VA_ARGS__)

//-----------------------------------//

/**
 * A mutex is used in concurrent programming to avoid the simultaneous
 * use of a common resource, such as a global variable, by pieces of 
 * computer code called critical sections.
 */

struct Mutex
{
#ifdef VAPOR_PLATFORM_WINDOWS
	CRITICAL_SECTION Handle;
#else
#endif
};

API_CORE Mutex* MutexCreate(Allocator*);
API_CORE void   MutexDestroy(Mutex*, Allocator*);
API_CORE void   MutexInit(Mutex*);
API_CORE void   MutexLock(Mutex*);
API_CORE void   MutexUnlock(Mutex*);

typedef scoped_ptr<Mutex, MutexDestroy> MutexPtr;
#define pMutexCreate(alloc, ...) CreateScopedPtr(MutexCreate, alloc, __VA_ARGS__)

//-----------------------------------//

struct Condition
{
#ifdef VAPOR_PLATFORM_WINDOWS
	CONDITION_VARIABLE Handle;
#else
#endif
};

API_CORE Condition* ConditionCreate(Allocator*);
API_CORE void       ConditionDestroy(Condition*, Allocator*);
API_CORE void       ConditionInit(Condition*);
API_CORE void       ConditionWait(Condition*, Mutex*);
API_CORE void       ConditionWakeOne(Condition*);
API_CORE void       ConditionWakeAll(Condition*);

typedef scoped_ptr<Condition, ConditionDestroy> ConditionPtr;
#define pConditionCreate(alloc, ...) CreateScopedPtr(ConditionCreate, alloc, __VA_ARGS__)

//-----------------------------------//

/**
 * Synchronizes access to variables that are shared by multiple threads.
 * Operations on these variables are performed atomically.
 * Remarks: On Windows this should be aligned to 32-bits.
 */

typedef int32 Atomic;

API_CORE int32 AtomicRead(volatile Atomic* atomic);
API_CORE int32 AtomicWrite(volatile Atomic* atomic, int32 value);
API_CORE int32 AtomicAdd(volatile Atomic* atomic, int32 value);
API_CORE int32 AtomicIncrement(volatile Atomic* atomic);
API_CORE int32 AtomicDecrement(volatile Atomic* atomic);

//-----------------------------------//

/**
 * Tasks provide an high level interface to the concurrency concepts.
 */

struct Task
{
	int16 Group;
	int16 Priority;
	Delegate1<Task*> Callback;
	void* Userdata;
};

API_CORE Task* TaskCreate(Allocator*);
API_CORE void  TaskDestroy(Task*, Allocator*);
API_CORE void  TaskRun(Task*);

typedef scoped_ptr<Task, TaskDestroy> TaskPtr;
#define pTaskCreate(alloc, ...) CreateScopedPtr(TaskCreate, alloc, __VA_ARGS__)

struct TaskEvent
{
	enum Enum { Added, Started, Finished };
	
	Task* task;
	TaskEvent::Enum event;
};

struct TaskPool
{
	std::vector<Thread*> Threads;
	ConcurrentQueue<Task*> Tasks;
	ConcurrentQueue<TaskEvent> Events;
	Event1<TaskEvent> OnTaskEvent;
	bool IsStopping;
};

API_CORE TaskPool*  TaskPoolCreate(Allocator*, int8 Size);
API_CORE void       TaskPoolDestroy(TaskPool*, Allocator*);
API_CORE void       TaskPoolAdd(TaskPool*, Task*);
API_CORE void       TaskPoolUpdate(TaskPool*);

typedef scoped_ptr<TaskPool, TaskPoolDestroy> TaskPoolPtr;
#define pTaskPoolCreate(alloc, ...) CreateScopedPtr(TaskPoolCreate, alloc, __VA_ARGS__)

//-----------------------------------//

NAMESPACE_EXTERN_END
