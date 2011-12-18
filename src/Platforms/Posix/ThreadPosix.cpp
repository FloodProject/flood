/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"

#ifndef VAPOR_PLATFORM_WINDOWS

#include "Thread.h"
#include "Log.h"

#include <stdlib.h>
#include <pthread.h>
#include <sched.h>

namespace vapor {
	
//-----------------------------------//
	
static void* _ThreadMain(void* ptr)
{	
	Thread* thread = (Thread*)ptr;
	thread->threadMain();
	
	return nullptr;
}

//-----------------------------------//

Thread::Thread()
	: mRunning(false)
	, mThread(0)
	, mPriority(ThreadPriority::Normal)
{ }

//-----------------------------------//

Thread::~Thread()
{
	assert(!mRunning);
}

//-----------------------------------//

bool Thread::startThread()
{
	if (mRunning)
		return false;

	mRunning = true;

	bool result = pthread_create((pthread_t*)&mThread, NULL, &_ThreadMain, this) == 0;

	if (result)
		setPriority(mPriority);
	
	return result;
}

//-----------------------------------//

void Thread::wait()
{
	assert(pthread_join((pthread_t)mThread, NULL) == 0);
	mRunning = false;
}

//-----------------------------------//

bool Thread::suspend() 
{
	assert( false && "Thread::Suspend Not Implemented" );
	return false;
}

//-----------------------------------//

bool Thread::resume() 
{
	assert( false && "Thread::Resume Not Implemented" );
	return false;
}

//-----------------------------------//

void Thread::setPriority(ThreadPriority::Enum priority) 
{
	// Update our internal state
	mPriority = priority;
	
	// Propogate to the thread if its running
	if ( mRunning )
	{
		int policy;
		sched_param param;
		memset(&param, 0, sizeof(param));
		
		// Get current policy
		assert( pthread_getschedparam( (pthread_t)mThread, &policy, &param ) == 0 );

		// Calculate priority range
		int max = sched_get_priority_max( policy );
		int min = sched_get_priority_min( policy );
		int range = max - min;
		int mid = range / 2;
		int new_priority = sched_get_priority_min( policy ) + mid + (int)priority;
		
		// Clear parameters
		memset(&param, 0, sizeof(param));
		param.sched_priority = new_priority;
		
		// Set new priority	
		assert( pthread_setschedparam( (pthread_t)mThread, policy, &param ) == 0 );
	}
}

//-----------------------------------//
	
bool Thread::isRunning() 
{
	return mRunning;
}

//-----------------------------------//

uintptr_t Thread::getCurrentThreadID()
{
	return (uintptr_t)pthread_self();
}

//-----------------------------------//
	
void Thread::threadMain()
{
	mInvoker->Invoke();
	delete mInvoker;
	mInvoker = nullptr;
	mRunning = false;
}

//-----------------------------------//
	
} // end namespace

#endif
