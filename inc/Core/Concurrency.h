/************************************************************************
*
* vapor3D Engine © (2008-2011)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/API.h"
#include "Event.h"
#include "ConcurrentQueue.h"

#ifdef VAPOR_PLATFORM_WINDOWS
	#define WIN32_LEAN_AND_MEAN
	#define NOMINMAX
	#include <Windows.h>
#endif

BEGIN_NAMESPACE_EXTERN

//-----------------------------------//

struct MemoryAllocator;

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

CORE_API Thread* ThreadCreate(MemoryAllocator*);
CORE_API void    ThreadDestroy(Thread*, MemoryAllocator*);
CORE_API bool    ThreadStart(Thread*, ThreadFunction, void*);
CORE_API bool    ThreadJoin(Thread*);
CORE_API bool    ThreadResume(Thread*);
CORE_API bool    ThreadStop(Thread*);
CORE_API bool    ThreadSetPriority(Thread*, ThreadPriority::Enum);
CORE_API void    ThreadSetName(Thread*, const String& name);

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

CORE_API Mutex* MutexCreate(MemoryAllocator*);
CORE_API void   MutexDestroy(Mutex*, MemoryAllocator*);
CORE_API void   MutexInit(Mutex*);
CORE_API void   MutexLock(Mutex*);
CORE_API void   MutexUnlock(Mutex*);

//-----------------------------------//

struct Condition
{
#ifdef VAPOR_PLATFORM_WINDOWS
	CONDITION_VARIABLE Handle;
#else
#endif
};

CORE_API Condition* ConditionCreate(MemoryAllocator*);
CORE_API void       ConditionDestroy(Condition*, MemoryAllocator*);
CORE_API void       ConditionInit(Condition*);
CORE_API void       ConditionWait(Condition*, Mutex*);
CORE_API void       ConditionWakeOne(Condition*);
CORE_API void       ConditionWakeAll(Condition*);

//-----------------------------------//

/**
 * Synchronizes access to variables that are shared by multiple threads.
 * Operations on these variables are performed atomically.
 * Remarks: On Windows this should be aligned to 32-bits.
 */

typedef int32 Atomic;

CORE_API int32 AtomicRead(volatile Atomic* atomic);
CORE_API int32 AtomicWrite(volatile Atomic* atomic, int32 value);
CORE_API int32 AtomicAdd(volatile Atomic* atomic, int32 value);
CORE_API int32 AtomicIncrement(volatile Atomic* atomic);
CORE_API int32 AtomicDecrement(volatile Atomic* atomic);

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

CORE_API Task* TaskCreate(MemoryAllocator*);
CORE_API void  TaskDestroy(Task*, MemoryAllocator*);
CORE_API void  TaskRun(Task*);

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

CORE_API TaskPool*  TaskPoolCreate(MemoryAllocator*, int8 Size);
CORE_API void       TaskPoolDestroy(TaskPool*, MemoryAllocator*);
CORE_API void       TaskPoolAdd(TaskPool*, Task*);
CORE_API void       TaskPoolUpdate(TaskPool*);

//-----------------------------------//

END_NAMESPACE_EXTERN
