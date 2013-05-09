/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Core/API.h"

#if !defined(PLATFORM_WINDOWS)

#include "Core/Concurrency.h"
#include "Core/Log.h"

#include <pthread.h>
#include <sched.h>

NAMESPACE_CORE_BEGIN

//-----------------------------------//

static void* _ThreadMain(void* ptr)
{	
	Thread* thread = (Thread*) ptr;
	thread->Function(thread, thread->Userdata);

	thread->Handle = nullptr;
	thread->IsRunning = false;

	return nullptr;
}

bool ThreadStart(Thread* thread, ThreadFunction function, void* data)
{
	if (!thread || !function || thread->IsRunning)
		return false;

	thread->Function = function;
	thread->Userdata = data;

	bool result = pthread_create((pthread_t*)&thread->Handle, /*attr=*/0,
		&_ThreadMain, thread) == 0;

	if (result)
	{
		thread->IsRunning = true;
		ThreadResume(thread);
	}
	
	return result;
}

bool ThreadJoin(Thread* thread)
{
	int res = pthread_join((pthread_t)thread->Handle, 0);

	assert(res == 0);
	thread->IsRunning = false;

	return res == 0;
}

bool ThreadPause(Thread* thread)
{
	assert(false && "ThreadPause not implemented");
	return false;
}

bool ThreadResume(Thread* thread)
{
	assert(false && "ThreadResume not implemented");
	return false;
}

bool ThreadSetPriority(Thread* thread, ThreadPriority priority)
{
	// Update our internal state
	thread->Priority = priority;
	int res = pthread_setschedprio((pthread_t)thread->Handle, (int) priority);

	return res == 0;

#if 0
	// Propogate to the thread if its running
	if (thread->IsRunning)
	{
		int policy;
		sched_param param;
		memset(&param, 0, sizeof(param));
		
		// Get current policy
		assert( pthread_getschedparam( (pthread_t)thread->Handle, &policy, &param ) == 0 );

		// Calculate priority range
		int max = sched_get_priority_max(policy);
		int min = sched_get_priority_min(policy);
		int range = max - min;
		int mid = range / 2;
		int new_priority = sched_get_priority_min(policy) + mid + (int)priority;
		
		// Clear parameters
		memset(&param, 0, sizeof(param));
		param.sched_priority = new_priority;
		
		// Set new priority
		int res = pthread_setschedparam((pthread_t)thread->Handle, policy, &param);
		assert(res == 0);
	}
#endif
}

void ThreadSetName(Thread*, const char* name)
{
	// TODO:
	//prctl(PR_SET_NAME);
}

//-----------------------------------//

struct Mutex
{
	pthread_mutex_t Handle;
};

Mutex* MutexCreate(Allocator* alloc)
{
	Mutex* mutex = Allocate(alloc, Mutex);
	MutexInit(mutex);

	return mutex;
}

void MutexInit(Mutex* mutex)
{
	if (!mutex) return;

	int res = pthread_mutex_init(&mutex->Handle, /*attr=*/0);
	assert((res == 0) && "Could not initialize critical section");
}

void MutexDestroy(Mutex* mutex)
{
	if (!mutex) return;

	int res = pthread_mutex_destroy(&mutex->Handle);
	assert((res == 0) && "Could not destroy critical section");

	Deallocate(mutex);
}

void MutexLock(Mutex* mutex)
{
	int res = pthread_mutex_lock(&mutex->Handle);
	assert((res == 0) && "Could not lock critical section");
}

void MutexUnlock(Mutex* mutex)
{
	int res = pthread_mutex_lock(&mutex->Handle);
	assert((res == 0) && "Could not unlock critical section");
}

//-----------------------------------//

struct Condition
{
	pthread_cond_t Handle;
};

Condition* ConditionCreate(Allocator* alloc)
{
	Condition* cond = Allocate(alloc, Condition);
	ConditionInit(cond);

	return cond;
}

void ConditionDestroy(Condition* cond)
{
	int res = pthread_cond_destroy(&cond->Handle);
	assert((res == 0) && "Could not destroy condition variable");

	Deallocate(cond);
}

void ConditionInit(Condition* cond)
{
	int res = pthread_cond_init(&cond->Handle, /*cond_attr=*/0);
	assert((res == 0) && "Could not destroy condition variable");
}

void ConditionWait(Condition* cond, Mutex* mutex)
{
	int res = pthread_cond_wait(&cond->Handle, &mutex->Handle);
	assert((res == 0) && "Could not wait on condition variable");
}

void ConditionWakeOne(Condition* cond)
{
	int res = pthread_cond_signal(&cond->Handle);
	assert((res == 0) && "Could not signal condition variable");
}

void ConditionWakeAll(Condition* cond)
{
	int res = pthread_cond_broadcast(&cond->Handle);
	assert((res == 0) && "Could not broadcast condition variable");
}

//-----------------------------------//

int32 AtomicRead(volatile Atomic* atomic)
{
	return __sync_add_and_fetch(atomic, 0);
}

int32 AtomicWrite(volatile Atomic* atomic, int32 value)
{
	return __sync_lock_test_and_set(atomic, value);
}

int32 AtomicAdd(volatile Atomic* atomic, int32 value)
{
	return __sync_add_and_fetch(atomic, value);
}

int32 AtomicIncrement(volatile Atomic* atomic)
{
	return __sync_add_and_fetch(atomic, 1);
}

int32 AtomicDecrement(volatile Atomic* atomic)
{
	return __sync_sub_and_fetch(atomic, 1);
}

//-----------------------------------//

NAMESPACE_CORE_END

#endif