//////////////////////////////////////////////////////////////////////////
//
// Author:  Corbin Hart
// Project: Genesis
// Date:    2/1/2011
//
// Pool - Implementation for the class that wraps the management of a group of
//	worker threads. Really, the Pool is more a common point of interaction than
//	a manager.
//
// Copyright 2010-2011 (All rights reserved)
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "Core/API.h"
#include "Concurrency.h"

NAMESPACE_CORE_BEGIN

	// -- Forward Declarations
	class Completion;
	class TaskBase;
	class WorkerThread;

	// -- Class Definition

	class API_CORE WorkerThreadPool
	{
		// -- Friends --
		friend class WorkerThread;

		// -- Member Data --
	private:
		//	- workers / shared data
		std::vector<WorkerThread *>		Workers_;				// array of Worker objects

		//	- synchronization stuff
		Completion * CurrentCompletion_;					// pointer to a (somewhat) current task's completion flag; used to synchronize
															//	 when threads should go idle and wake up; workers will ignore this until
															//	 they're ready for new work
		bool IsDone_;										// a notification flag designating that the pool wants to shut down
		
		MutexPtr PoolMutex_;								// mutex to provide the conditions something to lock around
		volatile Atomic ActiveWorkers_;						// number of active worker threads
		ConditionPtr AllWorkersIdle_;						// condition on which pool waits for workers to become idle
		ConditionPtr WakeupNotifiction_;					// condition on which all the idle threads are waiting

	public:
		WorkerThreadPool();

		void Make( uint32 threadCount );
		void Unmake();

		// <summary>
		// Will block until all of the worker threads (e.g. not the main thread) are idle.
		// </summary>
		INLINE void WaitUntilIdle()
		{
			if(AtomicRead(&ActiveWorkers_) == 1)
				return;

			MutexLock(PoolMutex_);
			ConditionWait(AllWorkersIdle_, PoolMutex_);
			MutexUnlock(PoolMutex_);
		}

		// <summary>
		// Signals any idle worker threads that there is new and more work to be done.
		// </summary>
		INLINE void WakeAll() 
		{
			ConditionWakeAll(WakeupNotifiction_);
		}

		// <summary>
		// Returns the number of total worker objects that are allocated. This is usually
		//	equivalent to boost::thread::hardware_concurrency()
		// </summary>
		INLINE uint32 WorkersCount() const { return Workers_.size(); }

		// <summary>
		// Returns the number of total worker threads that are actively working, or trying to
		//	steal work from other threads.
		//	NOTE: This will always be at least 1, as the main worker thread is always active.
		// </summary>
		INLINE Atomic ActiveWorkersCount() { return AtomicRead(&ActiveWorkers_); }

		// <summary>
		// Debug helper function, returning a pair of iterators representing the iterable range of
		//	the worker thread objects.
		// </summary>
		INLINE auto WorkersBeginEnd() 
			-> std::pair<std::vector<WorkerThread*>::iterator, std::vector<WorkerThread*>::iterator>
			//-> decltype( std::make_pair(Workers_.begin(), Workers_.end()) )
		{
			return std::make_pair(Workers_.begin(), Workers_.end());
		}
	};

NAMESPACE_CORE_END