//////////////////////////////////////////////////////////////////////////
//
// Flood © (2008-2013)
// Pool - Implementation for the pool of worker threads.
//
//////////////////////////////////////////////////////////////////////////

#include "Core/API.h"
#include "Core/Log.h"
#include "Core/Memory.h"

#include "Core/WorkerThreadPool.h"
#include "Core/WorkerThread.h"

NAMESPACE_CORE_BEGIN

#pragma region Ctr / Make / Unmake

	WorkerThreadPool::WorkerThreadPool()
		: CurrentCompletion_(nullptr)
		, IsDone_(false)
		, ActiveWorkers_(0)
	{}

	void WorkerThreadPool::Make( uint32 threadCount )
	{
		LogInfo("WorkerThreadPool is starting up. Thread count: %i", threadCount);

		// set up the initial data that we'll need
		CurrentCompletion_ = nullptr;
		IsDone_ = false;
		
		// create the resources used to synchronize worker idle state
		PoolMutex_.reset( MutexCreate(AllocatorGetHeap()) );
		AtomicAdd(&ActiveWorkers_, threadCount); 
		AllWorkersIdle_.reset( ConditionCreate(AllocatorGetHeap()) );
		
		// create wake up condition
		WakeupNotifiction_.reset( ConditionCreate(AllocatorGetHeap()) );

		// allocate the appropriate memory in the vector
		Workers_.reserve( threadCount );

		// allocate and make the worker objects
		for(uint32 i = 0; i < threadCount; ++i)
		{
			WorkerThread * w = new WorkerThread();
			w->Make(this, i);
			Workers_.push_back(w);
		}
	}

	void WorkerThreadPool::Unmake()
	{
		LogInfo("WorkerThreadPool shutting down...");

		// set the shutdown flag
		IsDone_ = true;

		// iterate through them all and unmake them
		WakeAll();
		for(auto w = Workers_.begin(); w != Workers_.end(); ++w)
			(*w)->Unmake();
	}

#pragma endregion

NAMESPACE_CORE_END