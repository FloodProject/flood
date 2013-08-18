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
	thread->function(thread, thread->userdata);

	thread->handle = nullptr;
	thread->isRunning = false;

	return nullptr;
}

bool Thread::start(ThreadFunction function, void* data)
{
	if (!function || isRunning)
		return false;

	this->function = function;
	userdata = data;

	bool result = pthreadcreate((pthreadt*)&handle, /*attr=*/0,
		&_ThreadMain, this) == 0;

	if (result)
	{
		isRunning = true;
		resume();
	}
	
	return result;
}

bool Thread::join()
{
	int res = pthreadjoin((pthreadt)handle, 0);

	assert(res == 0);
	isRunning = false;

	return res == 0;
}

bool Thread::pause()
{
	assert(false && "ThreadPause not implemented");
	return false;
}

bool Thread::resume()
{
	assert(false && "ThreadResume not implemented");
	return false;
}

bool Thread::setPriority(ThreadPriority priority)
{
	// Update our internal state
	this->priority = priority;
	int res = pthreadsetschedprio((pthreadt)handle, (int) priority);

	return res == 0;

#if 0
	// Propagate to the thread if it's running
	if (isRunning)
	{
		int policy;
		sched_param param;
		memset(&param, 0, sizeof(param));
		
		// Get current policy
		assert( pthreadgetschedparam( (pthreadt)handle, &policy, &param ) == 0 );

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
		int res = pthreadsetschedparam((pthreadt)handle, policy, &param);
		assert(res == 0);
	}
#endif
}

void Thread::setName(const char* name)
{
	// TODO:
	//prctl(PR_SET_NAME);
}

//-----------------------------------//
{
Mutex::Mutex()
{
	init();
}

Mutex::~Mutex()
{
	int res = pthreadmutex_destroy(handle);
	Deallocate(handle);
	assert((res == 0) && "Could not destroy critical section");
}

void Mutex::init()
{
	handle = AllocateHeap(pthreadmutex_t);
	int res = pthreadmutex_init(handle, /*attr=*/0);
	assert((res == 0) && "Could not initialize critical section");
}

void Mutex::lock()
{
	int res = pthreadmutex_lock(handle);
	assert((res == 0) && "Could not lock critical section");
}

void Mutex::unlock()
{
	int res = pthreadmutex_lock(handle);
	assert((res == 0) && "Could not unlock critical section");
}

//-----------------------------------//

Condition::Condition()
{
	init();
}

Condition::~Condition()
{
	int res = pthreadcond_destroy(handle);
	assert((res == 0) && "Could not destroy condition variable");
	Deallocate(handle);
}

void Condition::init()
{
	handle = AllocateHeap(pthreadcond_t);
	int res = pthreadcond_init(handle, /*cond_attr=*/0);
	assert((res == 0) && "Could not destroy condition variable");
}

void Condition::wait(Mutex& mutex)
{
	int res = pthreadcond_wait(handle, mutex.handle);
	assert((res == 0) && "Could not wait on condition variable");
}

void Condition::wakeOne()
{
	int res = pthreadcond_signal(handle);
	assert((res == 0) && "Could not signal condition variable");
}

void Condition::wakeAll()
{
	int res = pthreadcond_broadcast(handle);
	assert((res == 0) && "Could not broadcast condition variable");
}

//-----------------------------------//

NAMESPACE_CORE_END

#endif