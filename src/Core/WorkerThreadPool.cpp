/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Core/API.h"

#include "Core/WorkerThread.h"
#include "Core/WorkerThreadPool.h"
#include "Core/Memory.h"
#include "Core/Containers/Array.h"
#include "Core/Log.h"

NAMESPACE_CORE_BEGIN

#pragma region Ctr / Make / Unmake

	WorkerThreadPool::WorkerThreadPool()
		: CurrentCompletion_(nullptr)
		, IsDone_(false)
		, ActiveWorkers_(0)
		, Workers_(*AllocatorGetHeap())
	{}

	void WorkerThreadPool::Initialize( size_t threadCount )
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
		array::reserve(Workers_, threadCount );

		// allocate and make the worker objects
		for(uint32 i = 0; i < threadCount; ++i)
		{
			WorkerThread * w = new WorkerThread();
			w->Initialize(this, i);
			array::push_back(Workers_, w);
		}
	}

	void WorkerThreadPool::Shutdown()
	{
		LogInfo("WorkerThreadPool shutting down...");

		// set the shutdown flag
		IsDone_ = true;

		// iterate through them all and unmake them
		WakeAll();
		for(auto w = array::begin(Workers_); w != array::end(Workers_); ++w)
			(*w)->Shutdown();
	}

	size_t WorkerThreadPool::WorkersCount() const 
	{ 
		return array::size(Workers_);
	}

	std::pair<WorkerThread**, WorkerThread**> WorkerThreadPool::WorkersBeginEnd()
	{
		return std::make_pair(array::begin(Workers_), array::end(Workers_));
	}

#pragma endregion

NAMESPACE_CORE_END