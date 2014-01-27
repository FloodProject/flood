/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Core/API.h"
#include "Core/Event.h"
#include "Core/Pointers.h"
#include <vector>

#if defined(COMPILER_MSVC)
extern "C"
{
long __cdecl _InterlockedIncrement(long volatile * _Addend);
long __cdecl _InterlockedExchange(long volatile * _Target, long _Value);
long __cdecl _InterlockedExchangeAdd(long volatile * _Addend, long _Value);
}
#endif

#if defined(PLATFORM_WINDOWS)
typedef struct _RTL_CRITICAL_SECTION CRITICAL_SECTION;
typedef struct _RTL_CONDITION_VARIABLE CONDITION_VARIABLE;
#else
#include <pthread.h>
#endif

NAMESPACE_CORE_BEGIN

//-----------------------------------//

enum class ThreadPriority
{
	Low,
	Normal,
	High
};

class Thread;
typedef Delegate2<Thread*, void*> ThreadFunction;

/**
 * A thread is the entity within a process that can be scheduled for
 * execution. All threads of a process share its virtual address space
 * and system resources.
 */
class FLD_IGNORE API_CORE Thread
{
public:

	Thread();
	~Thread();

	/**
	 * Start thread execution.
	 * @param function function to run in the thread
	 * @param data function arguments
	 */
	bool start(ThreadFunction function, void* data);

	/**
	 * Blocks the calling thread until a thread terminates.
	 */
	bool join();

	/**
	 * Pause thread execution.
	 */
	bool pause();

	/**
	 * Resume thread execution.
	 */
	bool resume();

	/**
	 * Set thread priority.
	 * @param priority thread priority
	 */
	bool setPriority(ThreadPriority priority);

	/**
	 * Set thread name.
	 * @param name thread name
	 */

	void setName(const char* name);


	void* handle; //!< thread handle 
	volatile bool isRunning; //!< whether thread is running
	ThreadPriority priority; //!< thread priority
	ThreadFunction function; //!< function for the thread to run
	void* userdata; //!< function arguments
};

//-----------------------------------//

/**
 * A mutex is used in concurrent programming to avoid the simultaneous
 * use of a common resource, such as a global variable, by pieces of 
 * computer code called critical sections.
 */
struct FLD_IGNORE API_CORE Mutex
{
	Mutex();
	~Mutex();

	/**
	 * Init mutex.
	 */
	void init();

	/**
	 * Lock mutex.
	 */
	void lock();

	/**
	 * Unlock mutex.
	 */
	void unlock();

#if defined(PLATFORM_WINDOWS)
	CRITICAL_SECTION* handle; //!< mutex handle
#else
	pthreadmutex_t* handle; //!< mutex handle
#endif
};

//-----------------------------------//

/**
 * Condition variables allow threads to wait for certain 
 * events or conditions to occur and they notify other threads 
 * that are also waiting for the same events or conditions. 
 * The thread can wait on a condition variable and broadcast a 
 * condition such that one or all of the threads that are waiting on the 
 * condition variable become active.
 */
struct FLD_IGNORE API_CORE Condition
{
	Condition();
	~Condition();
	
	/**
	 * Init condition.
	 */
	void init();

	/**
	 * Block the calling thread until condition is signalled.
	 * @param mutex associated mutex
	 * @note the associated mutex is used to control access to the condition 
	 * variable. This method should be called while mutex is locked, 
	 * and it will automatically release the mutex while it waits. 
	 * After signal is received and thread is awakened, mutex will 
	 * be automatically locked for use by the thread. The programmer is 
	 * then responsible for unlocking mutex when the thread is finished 
	 * with it. 
	 */
	void wait(Mutex& mutex);
	
	/**
	 * Unblock one of the threads waiting on this condition variable.
	 */
	void wakeOne();

	/**
	 * Unblock all of the threads waiting on this condition variable.
	 */
	void wakeAll();

#if defined(PLATFORM_WINDOWS)
	CONDITION_VARIABLE* handle; //!< condition variable handle
#else
	pthreadcond_t* handle; //!< condition variable handle
#endif
};

//-----------------------------------//

/**
 * Synchronizes access to variables that are shared by multiple threads.
 * Operations on these variables are performed atomically.
 * @note type T must be castable to int32
 * On Windows this should be aligned to 32-bits.
 */
template<typename T> class Atomic
{
public:

	static_assert(sizeof(T) == 4, "T must be 32 bits");
	
	Atomic(const T& value = 0)
	{
		atomic = value;
	}

	Atomic(const Atomic<T>& other)
	{
		atomic = other.read();
	}

	Atomic<T> operator=(const Atomic<T>& value)
	{
		atomic = value.read();
		return *this;
	}

	/**
	 * Read the value of the atomic variable.
	 */
	T read() const
	{
#if defined(PLATFORM_WINDOWS)
		return ::_InterlockedExchangeAdd((volatile long*)&atomic, 0);
#else
		return __sync_add_and_fetch(const_cast<T*>(&atomic), 0);
#endif
	}

	/**
	 * Write a value to the atomic variable.
	 * @param value value to write 
	 */
	T write(const T& value)
	{
#if defined(PLATFORM_WINDOWS)
		return ::_InterlockedExchange((volatile long*)&atomic, value);
#else
		return __sync_lock_test_and_set(&atomic, value);
#endif
	}

	/**
	 * Add a value to the atomic variable.
	 * @param value value to add 
	 */
	T add(const T& value)
	{
#if defined(PLATFORM_WINDOWS)
		return ::_InterlockedExchangeAdd((volatile long*)&atomic, value);
#else
		return __sync_add_and_fetch(&atomic, value);
#endif
	}

	/**
	 * Increment the atomic variable.
	 */
	T increment()
	{
#if defined(PLATFORM_WINDOWS)
		return ::_InterlockedIncrement((volatile long*)&atomic);
#else
		return __sync_add_and_fetch(&atomic, 1);
#endif
	}

	/**
	 * Decrement the atomic variable.
	 */
	T decrement()
	{
#if defined(PLATFORM_WINDOWS)
		return ::_InterlockedDecrement((volatile long*)&atomic);
#else
		return __sync_sub_and_fetch(&atomic, 1);
#endif
	}

private:

	mutable T atomic; //!< atomic variable
};

//-----------------------------------//

NAMESPACE_CORE_END